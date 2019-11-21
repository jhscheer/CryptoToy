#include <jni.h>
#include <string>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <android/log.h>


#define  LOG_TAG    "FOO"

/* Encryption key */
const static unsigned char aes_key[]={0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF};
/* Input data to encrypt */
unsigned char input[]={0x1,0x2,0x3,0x4,0x1,0x2,0x3,0x4,0x1,0x2,0x3,0x4,0x1,0x2,0x3,0x4};


void print_data(const char *title, unsigned char* data, size_t len) {

    char *buffer;
    buffer = (char*)malloc(len * 3);
    for(unsigned int i=0; i<len; ++i){
        sprintf(&buffer[i*3], "%02x ", data[i]);
    }
    buffer[len*3-1] = '\0';
    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "%s: %s", title, buffer);

}

static void CBC_ENCRYPT() {
    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "%s", "AES_cbc_encrypt()");

    print_data("Original ", input, sizeof(input));

    /* Init vector */
    unsigned char iv[AES_BLOCK_SIZE];
    memset(iv, 0x00, AES_BLOCK_SIZE);

    /* Buffers for Encryption and Decryption */
    unsigned char enc_out[sizeof(input)];
    unsigned char dec_out[sizeof(input)];

    /* Encryption */
    AES_KEY enc_key, dec_key;
    AES_set_encrypt_key(aes_key, sizeof(aes_key)*8, &enc_key);
    AES_cbc_encrypt(input, enc_out, sizeof(input), &enc_key, iv, AES_ENCRYPT);

    print_data("Encrypted", enc_out, sizeof(enc_out));

    /* Decryption */
    memset(iv, 0x00, AES_BLOCK_SIZE); // don't forget to set iv vector again, else you can't decrypt data properly
    AES_set_decrypt_key(aes_key, sizeof(aes_key)*8, &dec_key); // Size of key is in bits
    AES_cbc_encrypt(enc_out, dec_out, sizeof(input), &dec_key, iv, AES_DECRYPT);

    print_data("Decrypted", dec_out, sizeof(dec_out));
}

static void DECRYPT() {

    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "%s", "AES_decrypt()");
    
    print_data("Original ", input, sizeof(input));

    /* Buffers for Encryption and Decryption */
    unsigned char enc_out[sizeof(input)];
    unsigned char dec_out[sizeof(input)];

    /* Encryption */
    AES_KEY enc_key, dec_key;
    AES_set_encrypt_key(aes_key, 128, &enc_key);
    AES_encrypt(input, enc_out, &enc_key);

    print_data("Encrypted", enc_out, sizeof(enc_out));

    /* Decryption */
    AES_set_decrypt_key(aes_key,128,&dec_key);
    AES_decrypt(enc_out, dec_out, &dec_key);

    print_data("Decrypted", dec_out, sizeof(dec_out));
}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    // Find your class. JNI_OnLoad is called from the correct class loader context for this to work.
    jclass c = env->FindClass("com/my/cryptotoy/MainActivity");

    if (c == nullptr) return JNI_ERR;

    // Register your class' native methods.
    static const JNINativeMethod methods[] = {
            {"aesDecrypt", "()V", (void*)DECRYPT},
            {"aesCbcEncrypt", "()V", (void*)CBC_ENCRYPT},
    };
    int rc = env->RegisterNatives(c, methods, sizeof(methods)/sizeof(JNINativeMethod));
    if (rc != JNI_OK) return rc;

    return JNI_VERSION_1_6;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_my_cryptotoy_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {

    #if defined(__arm__)
        #if defined(__ARM_ARCH_7A__)
        #if defined(__ARM_NEON__)
          #if defined(__ARM_PCS_VFP)
            #define ABI "armeabi-v7a/NEON (hard-float)"
          #else
            #define ABI "armeabi-v7a/NEON"
          #endif
        #else
          #if defined(__ARM_PCS_VFP)
            #define ABI "armeabi-v7a (hard-float)"
          #else
            #define ABI "armeabi-v7a"
          #endif
        #endif
      #else
       #define ABI "armeabi"
      #endif
    #elif defined(__i386__)
    #define ABI "x86"
    #elif defined(__x86_64__)
        #define ABI "x86_64"
    #elif defined(__mips64)  /* mips64el-* toolchain defines __mips__ too */
    #define ABI "mips64"
    #elif defined(__mips__)
    #define ABI "mips"
    #elif defined(__aarch64__)
    #define ABI "arm64-v8a"
    #else
    #define ABI "unknown"
    #endif


    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "%s (ABI: " ABI ")", SSLeay_version(SSLEAY_VERSION));


    return env->NewStringUTF("Hello from JNI ! Compiled with ABI " ABI ".");
}

