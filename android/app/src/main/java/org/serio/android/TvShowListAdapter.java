package org.serio.android;

import android.view.LayoutInflater;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import java.util.function.Consumer;

public class TvShowListAdapter extends RecyclerView.Adapter<TvShowViewHolder> {
    private boolean isLoaded;
    private Consumer<Integer> onItemFocused;

    public TvShowListAdapter(Consumer<Integer> onItemFocused) {
        this.onItemFocused = onItemFocused;
    }

    public void load() {
        isLoaded = true;
        notifyDataSetChanged();
    }

    @NonNull
    @Override
    public TvShowViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        return new TvShowViewHolder(LayoutInflater.from(parent.getContext()).inflate(R.layout.tv_show_card_fragment, parent, false));
    }

    @Override
    public void onBindViewHolder(@NonNull TvShowViewHolder holder, int position) {
        if (isLoaded) {
            String title = "Boruto: Naruto Next Generations";
            String subtitle = position % 2 == 0 ? position + " days ago" : null;
            String imageUrl = "https://thumbs.filmix.co/posters/orig/boruto-2017_117605_0.jpg";
            holder.bind(title, subtitle, imageUrl, () -> onItemFocused.accept(position));
        } else {
            holder.bind(null, null, null, () -> onItemFocused.accept(position));
        }
    }

    @Override
    public int getItemCount() {
        return 50;
    }
}
