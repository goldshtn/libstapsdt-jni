package sdt;

class Native {
	static {
		System.loadLibrary("stapsdt-jni");
	}

	public static native long providerInit(String name);
	public static native long providerAddProbe(long provider, String name, int... argTypes);
	public static native int providerLoad(long provider);
	public static native int providerUnload(long provider);
	public static native void providerDestroy(long provider);
	public static native void probeFire(long probe, Object... args);
	public static native boolean probeIsEnabled(long probe);
}


