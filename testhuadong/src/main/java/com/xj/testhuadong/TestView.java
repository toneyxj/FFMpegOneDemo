package com.xj.testhuadong;

import android.content.Context;
import android.util.AttributeSet;
import android.util.Log;
import android.widget.ImageView;

/**
 * Created by xj on 2018/7/10.
 */

public class TestView extends ImageView {
    public TestView(Context context) {
        super(context);
        initView();
    }

    public TestView(Context context, AttributeSet attrs) {
        super(context, attrs);
        initView();
    }

    public TestView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        initView();
    }
    public void initView(){
//        getLayoutParams().width=2160;
//        getLayoutParams().height=2800;
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        Log.e("widthMeasureSpec",String.valueOf(widthMeasureSpec));
        Log.e("heightMeasureSpec",String.valueOf(heightMeasureSpec));
        final int widthSpecMode = MeasureSpec.getMode(widthMeasureSpec);
        final int heightSpecMode = MeasureSpec.getMode(heightMeasureSpec);
        Log.e("widthSpecMode",String.valueOf(widthSpecMode));
        Log.e("heightSpecMode",String.valueOf(heightSpecMode));
        super.onMeasure(2160, 2800);
    }
}
