package org.serio.android;

import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.ActionBarDrawerToggle;
import androidx.appcompat.widget.Toolbar;
import androidx.core.view.GravityCompat;
import androidx.drawerlayout.widget.DrawerLayout;
import androidx.fragment.app.Fragment;
import androidx.recyclerview.widget.RecyclerView;

import com.facebook.shimmer.ShimmerFrameLayout;
import com.google.android.material.navigation.NavigationView;
import com.google.android.material.textview.MaterialTextView;

public class CategoryFragment extends Fragment implements NavigationView.OnNavigationItemSelectedListener, BackPressHandler {
    private ShimmerFrameLayout shimmerFrame;
    private RecyclerView tvShowList;
    private DrawerLayout drawerLayout;
    private MaterialTextView noTvShowsPlaceholder;
    private TvShowListAdapter adapter;

    public CategoryFragment() {
        super(R.layout.category_fragment);
    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        shimmerFrame = view.findViewById(R.id.category_shimmer_frame);
        tvShowList = view.findViewById(R.id.category_recycler_view);
        noTvShowsPlaceholder = view.findViewById(R.id.category_no_tv_shows_placeholder);
        Toolbar toolbar = view.findViewById(R.id.category_toolbar);
        toolbar.setTitle("Category Name");

        adapter = new TvShowListAdapter((item) -> tvShowList.smoothScrollToPosition(item));
        tvShowList.setAdapter(adapter);
        drawerLayout = view.findViewById(R.id.category_drawer);
        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(getActivity(), drawerLayout,
                toolbar, R.string.navigation_drawer_open, R.string.navigation_drawer_close);
        toggle.syncState();
        drawerLayout.addDrawerListener(toggle);

        NavigationView navigationView = view.findViewById(R.id.category_navigation_view);
        initializeNavigationMenu(navigationView.getMenu());
        navigationView.setNavigationItemSelectedListener(this);

        drawerLayout.addDrawerListener(new DrawerLayout.DrawerListener() {
            @Override
            public void onDrawerSlide(@NonNull View drawerView, float slideOffset) {

            }

            @Override
            public void onDrawerOpened(@NonNull View drawerView) {
                navigationView.requestFocus();
            }

            @Override
            public void onDrawerClosed(@NonNull View drawerView) {

            }

            @Override
            public void onDrawerStateChanged(int newState) {

            }
        });
        shimmerFrame.showShimmer(true);
        tvShowList.suppressLayout(true);
        view.postDelayed(this::displayTvShows, 3000);
//        view.postDelayed(this::displayNoTvShows, 3000);
    }

    @Override
    public boolean onNavigationItemSelected(@NonNull MenuItem item) {
        drawerLayout.closeDrawer(GravityCompat.START);
        return true;
    }

    @Override
    public boolean onBackPressed() {
        if (!drawerLayout.isDrawerOpen(GravityCompat.START)) {
            return false;
        } else {
            drawerLayout.closeDrawer(GravityCompat.START);
            return true;
        }
    }

    private void displayTvShows() {
        adapter.load();
        tvShowList.suppressLayout(false);
        shimmerFrame.hideShimmer();
    }

    private void displayNoTvShows() {
        shimmerFrame.setVisibility(View.GONE);
        noTvShowsPlaceholder.setVisibility(View.VISIBLE);
    }

    private void initializeNavigationMenu(Menu menu) {
        menu.clear();
        menu.add("Search");
        for (int i = 1; i <= 10; i++) {
            menu.add("Category #" + i);
        }
        menu.add("Settings");
        for (int i = 0; i < menu.size(); i++) {
            MenuItem item = menu.getItem(i);
            item.setCheckable(true);
        }
    }
}
