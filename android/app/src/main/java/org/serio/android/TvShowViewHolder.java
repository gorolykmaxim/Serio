package org.serio.android;

import android.content.Context;
import android.view.View;
import android.widget.ImageView;

import androidx.annotation.NonNull;
import androidx.core.content.ContextCompat;
import androidx.recyclerview.widget.RecyclerView;

import com.google.android.material.textview.MaterialTextView;
import com.squareup.picasso.Picasso;


public class TvShowViewHolder extends RecyclerView.ViewHolder {
    private final ImageView image;
    private final MaterialTextView title;
    private final MaterialTextView subtitle;

    public TvShowViewHolder(@NonNull View itemView) {
        super(itemView);
        image = itemView.findViewById(R.id.tv_show_card_image);
        title = itemView.findViewById(R.id.tv_show_card_title);
        subtitle = itemView.findViewById(R.id.tv_show_card_subtitle);
    }

    public void bind(String title, String subtitle, String imageUrl, Runnable onItemFocused) {
        itemView.setOnFocusChangeListener((v, hasFocus) -> {
            if (hasFocus) {
                onItemFocused.run();
            }
            v.post(() -> {
                this.title.setSelected(hasFocus);
                this.subtitle.setSelected(hasFocus);
            });
        });
        Context context = image.getContext();
        if (title == null && imageUrl == null) {
            image.setForeground(null);
            image.setBackground(ContextCompat.getDrawable(context, R.color.gray_500));
        } else {
            this.title.setText(title);
            if (subtitle != null) {
                this.subtitle.setVisibility(View.VISIBLE);
                this.subtitle.setText(subtitle);
            } else {
                this.subtitle.setVisibility(View.GONE);
            }
            image.setForeground(ContextCompat.getDrawable(context, R.drawable.bottom_shade_gradient));
            image.setBackground(null);
        }
        Picasso.get().load(imageUrl).into(image);
    }
}
