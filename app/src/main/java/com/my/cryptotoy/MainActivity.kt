package com.my.cryptotoy

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import androidx.annotation.Keep
import kotlinx.android.synthetic.main.activity_main.*


class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // Example of a call to a native method
        sample_text.text = stringFromJNI()
    }


    fun button1(view: View) {
        aesDecrypt()
    }

    fun button2(view: View) {
        aesCbcEncrypt()
    }

    companion object {
        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("crypto_1_0_2")
            System.loadLibrary("native-lib")
        }
    }

    /**
     * Native methods that are implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */

    // let the runtime look them up dynamically with dlsym
    external fun stringFromJNI(): String

    // explicitly register them with RegisterNatives
    external fun aesDecrypt()
    external fun aesCbcEncrypt()

}
