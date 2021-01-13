package org.serio.android;

import android.os.Bundle;
import android.view.View;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

public class CategoryFragment extends Fragment implements BackPressHandler {
    private CategoryNavigationPresenter navigationPresenter;

    public CategoryFragment() {
        super(R.layout.category_fragment);
    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        navigationPresenter = new CategoryNavigationPresenter(getActivity(), view);
        TvShowGridView gridView = view.findViewById(R.id.category_tv_show_grid);
        gridView.postDelayed(gridView::displayTvShows, 3000);
//        gridView.postDelayed(gridView::displayNoTvShows, 3000);
    }

    @Override
    public boolean onBackPressed() {
        return navigationPresenter.closeDrawerIfOpen();
    }
}
