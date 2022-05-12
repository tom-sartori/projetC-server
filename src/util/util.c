/**
 * Get the filepath.
 * @param filePath
 */
void getUploadDirectoryPath (char *filePath) {
    getwd(filePath);
    if (isMatch(filePath, "cmake-build-debug")) {
        // Program launched by Cmake project. It means that the current pwd is the directory cmake-build-debug.
        strcat(filePath, "/../uploads/");
    } else {
        // Program launched by the script client.sh.
        strcat(filePath, "/src/uploads/");
    }
}
