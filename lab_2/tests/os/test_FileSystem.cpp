#include <gtest/gtest.h>
#include "../../include/os/FileSystem.h"

class FileSystemTest : public ::testing::Test {
protected:
    FileSystem* fs;
    
    void SetUp() override {
        fs = new FileSystem("ext4");
    }
    
    void TearDown() override {
        delete fs;
    }
};

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ КОНСТРУКТОРА И ГЕТТЕРОВ
// ═══════════════════════════════════════════════════════════════════

TEST_F(FileSystemTest, InitialState) {
    EXPECT_EQ(fs->getType(), "ext4");
}

TEST_F(FileSystemTest, DifferentFileSystemTypes) {
    FileSystem* ntfs = new FileSystem("NTFS");
    FileSystem* fat32 = new FileSystem("FAT32");
    FileSystem* exfat = new FileSystem("exFAT");
    FileSystem* apfs = new FileSystem("APFS");
    FileSystem* btrfs = new FileSystem("btrfs");
    
    EXPECT_EQ(ntfs->getType(), "NTFS");
    EXPECT_EQ(fat32->getType(), "FAT32");
    EXPECT_EQ(exfat->getType(), "exFAT");
    EXPECT_EQ(apfs->getType(), "APFS");
    EXPECT_EQ(btrfs->getType(), "btrfs");
    
    delete ntfs;
    delete fat32;
    delete exfat;
    delete apfs;
    delete btrfs;
}

TEST_F(FileSystemTest, EmptyFileSystemType) {
    FileSystem* emptyFS = new FileSystem("");
    EXPECT_EQ(emptyFS->getType(), "");
    delete emptyFS;
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ CREATE FILE
// ═══════════════════════════════════════════════════════════════════

TEST_F(FileSystemTest, CreateSingleFile) {
    fs->createFile("test.txt", 1024);
    EXPECT_EQ(fs->getFileSize("test.txt"), 1024);
}

TEST_F(FileSystemTest, CreateMultipleFiles) {
    fs->createFile("file1.txt", 100);
    fs->createFile("file2.doc", 200);
    fs->createFile("file3.pdf", 300);
    
    EXPECT_EQ(fs->getFileSize("file1.txt"), 100);
    EXPECT_EQ(fs->getFileSize("file2.doc"), 200);
    EXPECT_EQ(fs->getFileSize("file3.pdf"), 300);
}

TEST_F(FileSystemTest, CreateFileWithZeroSize) {
    fs->createFile("empty.txt", 0);
    EXPECT_EQ(fs->getFileSize("empty.txt"), 0);
}

TEST_F(FileSystemTest, CreateFileWithLargeSize) {
    size_t largeSize = 1099511627776ULL;  // 1 TB
    fs->createFile("huge.iso", largeSize);
    EXPECT_EQ(fs->getFileSize("huge.iso"), largeSize);
}

TEST_F(FileSystemTest, CreateFileWithSpecialCharacters) {
    fs->createFile("file-name_123.tmp", 512);
    fs->createFile("my file.txt", 256);
    fs->createFile("document (1).pdf", 1024);
    
    EXPECT_EQ(fs->getFileSize("file-name_123.tmp"), 512);
    EXPECT_EQ(fs->getFileSize("my file.txt"), 256);
    EXPECT_EQ(fs->getFileSize("document (1).pdf"), 1024);
}

TEST_F(FileSystemTest, CreateFileWithLongName) {
    std::string longName(255, 'a');  // 255 символов
    longName += ".txt";
    fs->createFile(longName, 2048);
    EXPECT_EQ(fs->getFileSize(longName), 2048);
}

TEST_F(FileSystemTest, OverwriteExistingFile) {
    fs->createFile("test.txt", 1024);
    fs->createFile("test.txt", 2048);  // Перезапись
    
    EXPECT_EQ(fs->getFileSize("test.txt"), 2048);
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ GET FILE SIZE
// ═══════════════════════════════════════════════════════════════════

TEST_F(FileSystemTest, GetFileSizeExistingFile) {
    fs->createFile("document.pdf", 4096);
    EXPECT_EQ(fs->getFileSize("document.pdf"), 4096);
}

TEST_F(FileSystemTest, GetFileSizeNonExistentFile) {
    EXPECT_EQ(fs->getFileSize("notexists.txt"), 0);
}

TEST_F(FileSystemTest, GetFileSizeAfterMultipleCreates) {
    fs->createFile("file.dat", 100);
    EXPECT_EQ(fs->getFileSize("file.dat"), 100);
    
    fs->createFile("file.dat", 200);
    EXPECT_EQ(fs->getFileSize("file.dat"), 200);
    
    fs->createFile("file.dat", 300);
    EXPECT_EQ(fs->getFileSize("file.dat"), 300);
}

TEST_F(FileSystemTest, GetFileSizeEmptyFileName) {
    fs->createFile("", 512);
    EXPECT_EQ(fs->getFileSize(""), 512);
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ DELETE FILE
// ═══════════════════════════════════════════════════════════════════

TEST_F(FileSystemTest, DeleteExistingFile) {
    fs->createFile("temp.txt", 1024);
    fs->deleteFile("temp.txt");
    
    EXPECT_EQ(fs->getFileSize("temp.txt"), 0);
}

TEST_F(FileSystemTest, DeleteNonExistentFile) {
    fs->deleteFile("notexists.txt");  // Не должно вызывать ошибку
    EXPECT_EQ(fs->getFileSize("notexists.txt"), 0);
}

TEST_F(FileSystemTest, DeleteOneOfManyFiles) {
    fs->createFile("file1.txt", 100);
    fs->createFile("file2.txt", 200);
    fs->createFile("file3.txt", 300);
    
    fs->deleteFile("file2.txt");
    
    EXPECT_EQ(fs->getFileSize("file1.txt"), 100);
    EXPECT_EQ(fs->getFileSize("file2.txt"), 0);  // Удален
    EXPECT_EQ(fs->getFileSize("file3.txt"), 300);
}

TEST_F(FileSystemTest, DeleteAndRecreateFile) {
    fs->createFile("test.dat", 1024);
    fs->deleteFile("test.dat");
    fs->createFile("test.dat", 2048);
    
    EXPECT_EQ(fs->getFileSize("test.dat"), 2048);
}

TEST_F(FileSystemTest, DeleteAllFiles) {
    fs->createFile("file1.txt", 100);
    fs->createFile("file2.txt", 200);
    fs->createFile("file3.txt", 300);
    
    fs->deleteFile("file1.txt");
    fs->deleteFile("file2.txt");
    fs->deleteFile("file3.txt");
    
    EXPECT_EQ(fs->getFileSize("file1.txt"), 0);
    EXPECT_EQ(fs->getFileSize("file2.txt"), 0);
    EXPECT_EQ(fs->getFileSize("file3.txt"), 0);
}

TEST_F(FileSystemTest, DeleteSameFileTwice) {
    fs->createFile("file.txt", 512);
    fs->deleteFile("file.txt");
    fs->deleteFile("file.txt");  // Второй раз - нет ошибки
    
    EXPECT_EQ(fs->getFileSize("file.txt"), 0);
}

// ═══════════════════════════════════════════════════════════════════
// ИНТЕГРАЦИОННЫЕ ТЕСТЫ
// ═══════════════════════════════════════════════════════════════════

TEST_F(FileSystemTest, ComplexWorkflow) {
    // Создание
    fs->createFile("doc1.txt", 100);
    fs->createFile("doc2.txt", 200);
    fs->createFile("doc3.txt", 300);
    
    // Проверка
    EXPECT_EQ(fs->getFileSize("doc1.txt"), 100);
    EXPECT_EQ(fs->getFileSize("doc2.txt"), 200);
    EXPECT_EQ(fs->getFileSize("doc3.txt"), 300);
    
    // Удаление
    fs->deleteFile("doc2.txt");
    EXPECT_EQ(fs->getFileSize("doc2.txt"), 0);
    
    // Пересоздание
    fs->createFile("doc2.txt", 400);
    EXPECT_EQ(fs->getFileSize("doc2.txt"), 400);
    
    // Перезапись
    fs->createFile("doc1.txt", 500);
    EXPECT_EQ(fs->getFileSize("doc1.txt"), 500);
}

TEST_F(FileSystemTest, MixedOperations) {
    fs->createFile("a.txt", 10);
    fs->createFile("b.txt", 20);
    EXPECT_EQ(fs->getFileSize("a.txt"), 10);
    
    fs->deleteFile("a.txt");
    EXPECT_EQ(fs->getFileSize("a.txt"), 0);
    
    fs->createFile("c.txt", 30);
    EXPECT_EQ(fs->getFileSize("b.txt"), 20);
    EXPECT_EQ(fs->getFileSize("c.txt"), 30);
    
    fs->createFile("b.txt", 40);  // Overwrite
    EXPECT_EQ(fs->getFileSize("b.txt"), 40);
}

// ═══════════════════════════════════════════════════════════════════
// ГРАНИЧНЫЕ СЛУЧАИ
// ═══════════════════════════════════════════════════════════════════

TEST_F(FileSystemTest, VeryLargeFile) {
    size_t maxSize = SIZE_MAX;
    fs->createFile("maxfile.bin", maxSize);
    EXPECT_EQ(fs->getFileSize("maxfile.bin"), maxSize);
}

TEST_F(FileSystemTest, ManySmallFiles) {
    for (int i = 0; i < 1000; i++) {
        std::string filename = "file_" + std::to_string(i) + ".txt";
        fs->createFile(filename, i);
    }
    
    // Проверка нескольких
    EXPECT_EQ(fs->getFileSize("file_0.txt"), 0);
    EXPECT_EQ(fs->getFileSize("file_500.txt"), 500);
    EXPECT_EQ(fs->getFileSize("file_999.txt"), 999);
}

TEST_F(FileSystemTest, SameNameDifferentExtensions) {
    fs->createFile("file.txt", 100);
    fs->createFile("file.doc", 200);
    fs->createFile("file.pdf", 300);
    
    EXPECT_EQ(fs->getFileSize("file.txt"), 100);
    EXPECT_EQ(fs->getFileSize("file.doc"), 200);
    EXPECT_EQ(fs->getFileSize("file.pdf"), 300);
}

TEST_F(FileSystemTest, FileNameWithPath) {
    fs->createFile("/home/user/document.txt", 1024);
    fs->createFile("C:\\Users\\Admin\\file.doc", 2048);
    
    EXPECT_EQ(fs->getFileSize("/home/user/document.txt"), 1024);
    EXPECT_EQ(fs->getFileSize("C:\\Users\\Admin\\file.doc"), 2048);
}

TEST_F(FileSystemTest, UnicodeFileNames) {
    fs->createFile("файл.txt", 100);
    fs->createFile("文件.doc", 200);
    fs->createFile("αρχείο.pdf", 300);
    
    EXPECT_EQ(fs->getFileSize("файл.txt"), 100);
    EXPECT_EQ(fs->getFileSize("文件.doc"), 200);
    EXPECT_EQ(fs->getFileSize("αρχείο.pdf"), 300);
}

// ═══════════════════════════════════════════════════════════════════
// СТРЕСС-ТЕСТЫ
// ═══════════════════════════════════════════════════════════════════

TEST_F(FileSystemTest, CreateDeleteManyTimes) {
    for (int i = 0; i < 100; i++) {
        fs->createFile("temp.txt", i);
        EXPECT_EQ(fs->getFileSize("temp.txt"), i);
        fs->deleteFile("temp.txt");
        EXPECT_EQ(fs->getFileSize("temp.txt"), 0);
    }
}

TEST_F(FileSystemTest, ThousandFiles) {
    // Создание 1000 файлов
    for (int i = 0; i < 1000; i++) {
        fs->createFile("file" + std::to_string(i), i * 10);
    }
    
    // Проверка случайных
    EXPECT_EQ(fs->getFileSize("file0"), 0);
    EXPECT_EQ(fs->getFileSize("file100"), 1000);
    EXPECT_EQ(fs->getFileSize("file500"), 5000);
    EXPECT_EQ(fs->getFileSize("file999"), 9990);
    
    // Удаление половины
    for (int i = 0; i < 500; i++) {
        fs->deleteFile("file" + std::to_string(i * 2));
    }
    
    // Проверка
    EXPECT_EQ(fs->getFileSize("file0"), 0);    // Удален
    EXPECT_EQ(fs->getFileSize("file1"), 10);   // Остался
    EXPECT_EQ(fs->getFileSize("file100"), 0);  // Удален
    EXPECT_EQ(fs->getFileSize("file101"), 1010); // Остался
}

// ═══════════════════════════════════════════════════════════════════
// ПРОВЕРКА ТИПА ФАЙЛОВОЙ СИСТЕМЫ
// ═══════════════════════════════════════════════════════════════════

TEST_F(FileSystemTest, TypePersistsAfterOperations) {
    EXPECT_EQ(fs->getType(), "ext4");
    
    fs->createFile("file.txt", 100);
    EXPECT_EQ(fs->getType(), "ext4");
    
    fs->deleteFile("file.txt");
    EXPECT_EQ(fs->getType(), "ext4");
    
    fs->createFile("another.doc", 200);
    fs->createFile("more.pdf", 300);
    EXPECT_EQ(fs->getType(), "ext4");
}

TEST_F(FileSystemTest, DifferentFileSystemsIndependent) {
    FileSystem* fs1 = new FileSystem("ext4");
    FileSystem* fs2 = new FileSystem("NTFS");
    
    fs1->createFile("file.txt", 100);
    fs2->createFile("file.txt", 200);
    
    EXPECT_EQ(fs1->getFileSize("file.txt"), 100);
    EXPECT_EQ(fs2->getFileSize("file.txt"), 200);
    EXPECT_EQ(fs1->getType(), "ext4");
    EXPECT_EQ(fs2->getType(), "NTFS");
    
    delete fs1;
    delete fs2;
}
