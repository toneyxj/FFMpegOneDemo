/*****************************
 * *                                                                            **
 * *                     Jiedi(China nanjing)Ltd.                               **
 * *	               创建：夏曹俊，此代码可用作为学习参考                       **
 * FILE INFOMATION***********************************
 * *
 * * Project       : FFmpeg
 * * Description   : FFMPEG项目创建示例
 * * Contact       : xiacaojun@qq.com
 * *        博客   : http://blog.csdn.net/jiedichina
 * *		视频课程 : 网易云课堂	http://study.163.com/u/xiacaojun
 * 腾讯课堂		https://jiedi.ke.qq.com/
 * csdn学院		http://edu.csdn.net/lecturer/lecturer_detail?lecturer_id=961
 * *                 51cto学院	http://edu.51cto.com/lecturer/index/user_id-12016059.html
 * * 				   下载最新的ffmpeg版本 ffmpeg.club
 * *
 * *   安卓流媒体播放器 课程群 ：23304930 加入群下载代码和交流
 * *   微信公众号  : jiedi2007
 * *		头条号	 : 夏曹俊
 * *
 *******************************************************************************/

/*****************************FILE INFOMATION***********************************
 **
 ** Project       : FFmpeg
 ** Description   : FFMPEG项目创建示例
 ** Contact       : xiacaojun@qq.com
 **        博客   : http://blog.csdn.net/jiedichina
 **		视频课程 : 网易云课堂	http://study.163.com/u/xiacaojun
 腾讯课堂		https://jiedi.ke.qq.com/
 csdn学院		http://edu.csdn.net/lecturer/lecturer_detail?lecturer_id=961
 **                 51cto学院	http://edu.51cto.com/lecturer/index/user_id-12016059.html
 ** 				   下载最新的ffmpeg版本 ffmpeg.club
 **
 **   安卓流媒体播放器 课程群 ：23304930 加入群下载代码和交流
 **   微信公众号  : jiedi2007
 **		头条号	 : 夏曹俊
 **
 *******************************************************************************/
//！！！！！！！！！ 加群23304930下载代码和交流


package com.xj.ffmpegonedemo;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

/**
 * Created by Administrator on 2018-03-11.
 */

public class OpenUrl extends Activity implements View.OnClickListener {
    Button playvideo;
    Button playrtmp;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.openurl);
        playvideo = (Button) findViewById(R.id.playvideo);
        playrtmp = (Button) findViewById(R.id.playrtmp);
        playvideo.setOnClickListener(this);
        playrtmp.setOnClickListener(this);

    }

    @Override
    public void onClick(View v) {
        String path=null;
        switch (v.getId()) {
            case R.id.playvideo:
                path=((EditText)findViewById(R.id.fileurl)).getText().toString();
                break;
            case R.id.playrtmp:
                path=((EditText)findViewById(R.id.rtmpurl)).getText().toString();
                break;
            default:
                break;
        }
        if (path!=null){
            Intent intent=new Intent();
            intent.putExtra("path",path);
            setResult(RESULT_OK,intent);
            this.finish();
        }
    }


}

