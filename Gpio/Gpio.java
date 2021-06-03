class Gpio {
    static {
        System.loadLibrary("Gpio");
    }

    public native int open(int gpio);
    public native void close(int gpio);

    public native void setDirection(int gpio, int direction);

    public native int getValue(int gpio);
    public native void setValue(int gpio, int value);
}
