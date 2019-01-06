package com.cmejia.adoptame.ViewHolders;

import android.support.annotation.NonNull;
import android.support.v7.widget.RecyclerView;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

import com.cmejia.adoptame.Clases.Brand;
import com.cmejia.adoptame.R;


public class BrandViewHolder extends RecyclerView.ViewHolder {
    private TextView brandDesc;
    private ImageView brandImg;

    private BrandViewHolder.ClickListener listener;

    public BrandViewHolder(@NonNull View itemView) {
        super(itemView);

        brandDesc = itemView.findViewById(R.id.desc_brand);
        brandImg = itemView.findViewById(R.id.img_brand);

        itemView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                listener.onItemClick(v, getAdapterPosition());
            }
        });
    }

    public ImageView getBrandImg()
    {
        return brandImg;
    }

    public TextView getBrandDesc()
    {
        return brandDesc;
    }

    public void setBrandImg(ImageView brandImg)
    {
        this.brandImg = brandImg;
    }

    public void setBrandDesc(TextView brandDesc)
    {
        this.brandDesc = brandDesc;
    }

    public void bindBrand(Brand item)  {
        brandDesc.setText(item.getDescripcion());
        brandImg.setImageResource(item.getImage());
    }

    //Interface to send callbacks...
    public interface ClickListener {
        public void onItemClick(View view, int position);
    }

    public void setOnClickListener(BrandViewHolder.ClickListener clickListener){
        listener = clickListener;
    }

}
