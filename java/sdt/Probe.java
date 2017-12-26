package sdt;

public class Probe {
	public static final int NOARG = 0;
	public static final int UINT8 = 1;
	public static final int INT8 = -1;
	public static final int UINT16 = 2;
	public static final int INT16 = -2;
	public static final int UINT32 = 4;
	public static final int INT32 = -4;
	public static final int UINT64 = 8;
	public static final int INT64 = -8;
	public static final int STRING = UINT64;

	private final long probeHandle;

	public Probe(long probeHandle) {
		this.probeHandle = probeHandle;
	}

	public boolean isEnabled() {
		return Native.probeIsEnabled(this.probeHandle);
	}

	public void fire(Object... arguments) {
		Native.probeFire(this.probeHandle, arguments);
	}
}
