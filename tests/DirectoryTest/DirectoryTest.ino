#line 2 "DirectoryTest.ino"

#include <Arduino.h>
#include <AUnit.h>
#include <AceCommon.h> // PrintStr

using ace_common::PrintStr;

#if defined(UNIX_HOST_DUINO)
  #include <UnixHostFS.h>
#elif defined(ESP8266)
  #inclue <LittleFS.h>
#else
  #error Unsupported platform
#endif

using aunit::TestRunner;
using namespace fs;

//---------------------------------------------------------------------------

#if defined(UNIX_HOST_DUINO)
  FS& FILE_SYSTEM = UnixHostFS;
#elif defined(ESP8266)
  FS& FILE_SYSTEM = LittleFS;
#else
  #error Unsupported platform
#endif

static const char FILE_NAME[] = "testfile.txt";
static const char FILE_NAME2[] = "testfile2.txt";
static const char TEXT[] = "This is a test";

//---------------------------------------------------------------------------
// Tests for UnixHostFileImpl
//---------------------------------------------------------------------------

test(UnixHostDirImpl, openDir) {
  Dir dir = FILE_SYSTEM.openDir("/");
  int count = 0;
  while (dir.next()) {
    count++;
  }
  // Each directory contains "." and ".."
  assertEqual(2, count);
}

//---------------------------------------------------------------------------
// Tests for UnixHostFileImpl
//---------------------------------------------------------------------------

static void writeFile(const char* fileName, const char* text) {
  File f = FILE_SYSTEM.open(fileName, "w");
  f.print(text);
  f.close();
}

static void readFileInto(File& f, Print& printStr) {
  while (f.available()) {
    int c = f.read();
    printStr.write(c);
  }
}

test(UnixHostFileImplTest, writeFile_readFile) {
  writeFile(FILE_NAME, TEXT);

  File f = FILE_SYSTEM.open(FILE_NAME, "r");
  PrintStr<64> printStr;
  readFileInto(f, printStr);
  f.close();

  assertEqual(TEXT, printStr.getCstr());
}

test(UnixHostFileImplTest, seekFile) {
  writeFile(FILE_NAME, TEXT);

  File f = FILE_SYSTEM.open(FILE_NAME, "r");
  PrintStr<64> printStr;
  f.seek(5);
  readFileInto(f, printStr);
  f.close();

  assertEqual(F("is a test"), printStr.getCstr());
}

test(UnixHostFileImplTest, truncate) {
  writeFile(FILE_NAME, TEXT);
  File f = FILE_SYSTEM.open(FILE_NAME, "r");
  bool status = f.truncate(0);
  assertTrue(status);
  assertEqual((size_t) 0, f.size());
}

test(UnixHostFileImplTest, validateProperties) {
  writeFile(FILE_NAME, TEXT);

  File f = FILE_SYSTEM.open(FILE_NAME, "r");
  assertEqual(sizeof(TEXT) - 1, f.size());
  assertTrue(f.isFile());
  assertFalse(f.isDirectory());
  f.close();
}

//---------------------------------------------------------------------------
// Tests for UnixHostFSImpl
//---------------------------------------------------------------------------

test(UnixHostFSImplTest, exists) {
  writeFile(FILE_NAME, TEXT);
  assertTrue(FILE_SYSTEM.exists(FILE_NAME));
}

test(UnixHostFSImplTest, rename) {
  writeFile(FILE_NAME, TEXT);

  FILE_SYSTEM.rename(FILE_NAME, FILE_NAME2);
  assertFalse(FILE_SYSTEM.exists(FILE_NAME));
  assertTrue(FILE_SYSTEM.exists(FILE_NAME2));
}

//---------------------------------------------------------------------------

bool setupStatus = true;

void setup() {
#if ! defined(UNIX_HOST_DUINO)
  delay(1000); // wait to prevent garbage on SERIAL_PORT_MONITOR
#endif

  SERIAL_PORT_MONITOR.begin(115200);
  while (!SERIAL_PORT_MONITOR); // needed for Leonardo/Micro

  if (FILE_SYSTEM.begin()){
    SERIAL_PORT_MONITOR.println(F("FileSystem initialized."));
  } else {
    SERIAL_PORT_MONITOR.println(F("FileSystem init failed"));
    setupStatus = false;
  }

  // Start each test with a clean slate.
  if (setupStatus) {
    FILE_SYSTEM.format();
  }
}

void loop() {
  if (setupStatus) {
    TestRunner::run();
  }
}