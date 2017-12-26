#include <stdint.h>

extern "C" {
#include <libstapsdt.h>
}

#include "sdt_Native.h"

class globals {
public:
	static globals &instance(JNIEnv *env) {
		static globals inst(env);
		return inst;
	}
	bool is_long(jobject obj) const {
		return env_->IsInstanceOf(obj, long_class_);
	}
	bool is_int(jobject obj) const {
		return env_->IsInstanceOf(obj, int_class_);
	}
	bool is_string(jobject obj) const {
		return env_->IsInstanceOf(obj, string_class_);
	}
	jlong get_long(jobject obj) const {
		return env_->CallLongMethod(obj, long_value_method_);
	}
	jint get_int(jobject obj) const {
		return env_->CallIntMethod(obj, int_value_method_);
	}
private:
	globals(JNIEnv *env) : env_(env) {
		int_class_ = (jclass)env->NewGlobalRef(env->FindClass("java/lang/Integer"));
		long_class_ = (jclass)env->NewGlobalRef(env->FindClass("java/lang/Long"));
		string_class_ = (jclass)env->NewGlobalRef(env->FindClass("java/lang/String"));
		int_value_method_ = env->GetMethodID(int_class_, "intValue", "()I");
		long_value_method_ = env->GetMethodID(long_class_, "longValue", "()J");
	}
	JNIEnv *env_;
	jclass int_class_;
	jclass long_class_;
	jclass string_class_;
	jmethodID int_value_method_;
	jmethodID long_value_method_;
};

JNIEXPORT jlong JNICALL Java_sdt_Native_providerInit(JNIEnv *env, jclass cls, jstring name) {
	const char *provider_name = env->GetStringUTFChars(name, 0);
	SDTProvider_t *provider = providerInit(provider_name);
	env->ReleaseStringUTFChars(name, provider_name);
	return (jlong)provider;
}

JNIEXPORT jlong JNICALL Java_sdt_Native_providerAddProbe(JNIEnv *env, jclass cls, jlong provider,
						       jstring name, jintArray args) {
	jint *args_ptr = env->GetIntArrayElements(args, NULL);
	jsize args_length = env->GetArrayLength(args);
	const char *probe_name = env->GetStringUTFChars(name, 0);
	// MAX_ARGUMENTS is currently 6. Any extra arguments beyond args_length are discarded.
	SDTProbe_t *probe = providerAddProbe(
		(SDTProvider_t *)provider, probe_name, args_length,
		args_ptr[0], args_ptr[1], args_ptr[2], args_ptr[3], args_ptr[4], args_ptr[5]);
	env->ReleaseStringUTFChars(name, probe_name);
	return (jlong)probe;
}

JNIEXPORT jint JNICALL Java_sdt_Native_providerLoad(JNIEnv *env, jclass cls, jlong provider) {
	return (jint)providerLoad((SDTProvider_t *)provider);
}

JNIEXPORT jint JNICALL Java_sdt_Native_providerUnload(JNIEnv *env, jclass cls, jlong provider) {
	return (jint)providerUnload((SDTProvider_t *)provider);
}

JNIEXPORT void JNICALL Java_sdt_Native_providerDestroy(JNIEnv *env, jclass cls, jlong provider) {
	providerDestroy((SDTProvider_t *)provider);
}

JNIEXPORT void JNICALL Java_sdt_Native_probeFire(JNIEnv *env, jclass cls,
					       jlong probe, jobjectArray args) {
	globals& globs = globals::instance(env);

	// Marshal the arguments: everything numeric becomes uint64, strings need to be
	// allocated from the array and sent to the probe.
	uint64_t args_marshaled[MAX_ARGUMENTS] = {0};
	const char* strings_to_free[MAX_ARGUMENTS] = {0};
	jsize args_length = env->GetArrayLength(args);
	for (int i = 0; i < args_length; ++i) {
		jobject obj = env->GetObjectArrayElement(args, i);
		if (globs.is_int(obj)) {
			args_marshaled[i] = (uint64_t)globs.get_int(obj);
		} else if (globs.is_long(obj)) {
			args_marshaled[i] = (uint64_t)globs.get_long(obj);
		} else if (globs.is_string(obj)) {
			const char *string_value = env->GetStringUTFChars((jstring)obj, 0);
			args_marshaled[i] = (uint64_t)string_value;
			strings_to_free[i] = string_value;
		} else {
			args_marshaled[i] = 0;
		}
	}

	// Fire the probe; the implementation discards any arguments beyond the probe's
	// registered number of arguments.
	probeFire((SDTProbe_t *)probe, args_marshaled[0], args_marshaled[1], args_marshaled[2],
				       args_marshaled[3], args_marshaled[4], args_marshaled[5]);

	for (int i = 0; i < args_length; ++i) {
		if (strings_to_free[i] != 0) {
			jobject obj = env->GetObjectArrayElement(args, i);
			env->ReleaseStringUTFChars((jstring)obj, strings_to_free[i]);
		}
	}
}

JNIEXPORT jboolean JNICALL Java_sdt_Native_probeIsEnabled(JNIEnv *env, jclass cls, jlong probe) {
	return 1 == (jint)probeIsEnabled((SDTProbe_t *)probe);
}
