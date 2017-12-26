package sdt;

class Demo {
	public static void main(String[] args) throws Exception {
		Provider provider = new Provider("jprovider");
		Probe probe = provider.addProbe("jprobe", Probe.INT32, Probe.INT64, Probe.STRING);
		provider.load();
		for (int i = 0; i < 10000; ++i) {
			Thread.sleep(1000);
			if (probe.isEnabled()) {
				System.out.println("Firing probe #" + i);
				probe.fire(i, i*i, String.format("iteration #%d done", i));
			} else {
				System.out.println("Probe is not enabled");
			}
		}
		provider.unload();
		provider.destroy();
	}
}
