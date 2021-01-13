package org.serio.android;

import android.content.Context;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.View;

import com.facebook.shimmer.ShimmerFrameLayout;
import com.google.android.material.textview.MaterialTextView;

public class TvShowGridView extends ShimmerFrameLayout {
    private GridRecyclerView tvShowGrid;
    private TvShowListAdapter adapter;
    private MaterialTextView noTvShowsPlaceholder;

    public TvShowGridView(Context context, AttributeSet attrs) {
        super(context, attrs);
        initialize(context);
    }

    private void initialize(Context context) {
        LayoutInflater.from(context).inflate(R.layout.tv_show_grid_view, this);
        tvShowGrid = findViewById(R.id.tv_show_grid_recycler_view);
        adapter = new TvShowListAdapter(tvShowGrid::smoothScrollToPosition);
        tvShowGrid.setAdapter(adapter);
        noTvShowsPlaceholder = findViewById(R.id.tv_show_grid_no_tv_shows_placeholder);
        disableGrid();
        toggleVisibility(true);
    }

    public void displayTvShows() {
        adapter.load();
        enableGrid();
        toggleVisibility(true);
    }

    public void displayNoTvShows() {
        enableGrid();
        toggleVisibility(false);
    }

    private void enableGrid() {
        tvShowGrid.suppressLayout(false);
        hideShimmer();
    }

    private void disableGrid() {
        showShimmer(true);
        tvShowGrid.suppressLayout(true);
    }

    private void toggleVisibility(boolean hasTvShows) {
        tvShowGrid.setVisibility(hasTvShows ? View.VISIBLE : View.GONE);
        noTvShowsPlaceholder.setVisibility(hasTvShows ? View.GONE : View.VISIBLE);
    }
}
