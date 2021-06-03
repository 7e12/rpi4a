class Uart {
    static {
        System.loadLibrary("Uart");
    }

    public native int open(int uart, int baud_rate);
    public native void close(int file_dir);

    public native int read(int file_dir, byte[] buffer, int length);
    public native void write(int file_dir, byte[] buffer, int length);

    public native int isDataAvailable(int file_dir);
    public native void flush(int file_dir);
}
