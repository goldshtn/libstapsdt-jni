package sdt;

public class Provider {
	private final long providerHandle;
	private final String name;
	private boolean loaded;

	public Provider(String name) {
		this.name = name;
		this.providerHandle = Native.providerInit(this.name);
		if (providerHandle == 0) {
			throw new SDTException("Unable to initialize provider: " + this.name);
		}
	}

	public Probe addProbe(String probeName, int... argumentTypes) {
		long probeHandle = Native.providerAddProbe(
				this.providerHandle, probeName, argumentTypes);
		if (probeHandle == 0) {
			throw new SDTException("Unable to add probe: " + probeName);
		}
		return new Probe(probeHandle);
	}

	public void load() {
		if (0 != Native.providerLoad(this.providerHandle)) {
			throw new SDTException("Unable to load provider: " + this.name);
		}
		this.loaded = true;
	}

	public void unload() {
		if (!this.loaded) {
			throw new SDTException("Attempted to unload a provider that was not " +
					       "loaded: " + this.name);
		}
		if (0 != Native.providerUnload(this.providerHandle)) {
			throw new SDTException("Unable to unload provider: " + this.name);
		}
		this.loaded = false;
	}

	public void destroy() {
		if (this.loaded) {
			unload();
		}
		Native.providerDestroy(this.providerHandle);
	}
}
