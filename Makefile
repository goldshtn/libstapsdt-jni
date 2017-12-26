all:
	@# Build libstapsdt.so
	mkdir -p out
	sudo docker build -t stap-java-build build
	@# Build the Java package and demo application, and then run it
	sudo docker run --rm -v $(CURDIR)/out:/out stap-java-build
	sudo docker build -t stap-java-jni java
	@# The /tmp mapping is needed for the uprobe attach to work in a container
	sudo docker run -it --rm -v $(CURDIR)/out:/out -v /tmp:/tmp stap-java-jni

clean:
	rm -rf out
