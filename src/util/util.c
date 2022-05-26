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

void strcatArray(char *buffer, int bufferSize, char *tab[], int tabSize) {
    bzero(buffer, bufferSize);

    for (int i = 0; i < tabSize; i++) {
        strcat(buffer, tab[i]);
    }
}
