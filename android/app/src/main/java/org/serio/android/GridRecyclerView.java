package org.serio.android;

import android.content.Context;
import android.content.res.TypedArray;
import android.util.AttributeSet;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.recyclerview.widget.GridLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

public class GridRecyclerView extends RecyclerView {
    private GridLayoutManager manager;
    private int columnWidth = -1;

    public GridRecyclerView(@NonNull Context context) {
        super(context);
        initialize(context, null);
    }

    public GridRecyclerView(@NonNull Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        initialize(context, attrs);
    }

    public GridRecyclerView(@NonNull Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        initialize(context, attrs);
    }

    private void initialize(Context context, AttributeSet attrs) {
        if (attrs != null) {
            TypedArray values = context.obtainStyledAttributes(attrs, new int[] {android.R.attr.columnWidth});
            columnWidth = values.getDimensionPixelSize(0, -1);
            values.recycle();
        }
        manager = new GridLayoutManager(context, -1);
        setLayoutManager(manager);
    }

    @Override
    protected void onMeasure(int widthSpec, int heightSpec) {
        super.onMeasure(widthSpec, heightSpec);
        if (columnWidth > 0) {
            manager.setSpanCount(Math.max(2, getMeasuredWidth() / columnWidth));
        }
    }
}
