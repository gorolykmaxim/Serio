package org.serio.android;

import android.app.Activity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;

import androidx.annotation.NonNull;
import androidx.appcompat.app.ActionBarDrawerToggle;
import androidx.appcompat.widget.Toolbar;
import androidx.core.view.GravityCompat;
import androidx.drawerlayout.widget.DrawerLayout;

import com.google.android.material.navigation.NavigationView;

public class CategoryNavigationPresenter implements NavigationView.OnNavigationItemSelectedListener, DrawerLayout.DrawerListener {
    private final DrawerLayout drawerLayout;
    private final NavigationView navigationView;
    private final Toolbar toolbar;

    public CategoryNavigationPresenter(Activity activity, View view) {
        toolbar = view.findViewById(R.id.category_toolbar);
        toolbar.setTitle("Category Name");
        drawerLayout = view.findViewById(R.id.category_drawer);
        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(activity, drawerLayout,
                toolbar, R.string.navigation_drawer_open, R.string.navigation_drawer_close);
        toggle.syncState();
        drawerLayout.addDrawerListener(toggle);
        navigationView = view.findViewById(R.id.category_navigation_view);
        initializeNavigationMenu(navigationView.getMenu());
        navigationView.setNavigationItemSelectedListener(this);
        drawerLayout.addDrawerListener(this);
    }

    public boolean closeDrawerIfOpen() {
        if (!drawerLayout.isDrawerOpen(GravityCompat.START)) {
            return false;
        } else {
            drawerLayout.closeDrawer(GravityCompat.START);
            return true;
        }
    }

    @Override
    public boolean onNavigationItemSelected(@NonNull MenuItem item) {
        drawerLayout.closeDrawer(GravityCompat.START);
        return true;
    }

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
