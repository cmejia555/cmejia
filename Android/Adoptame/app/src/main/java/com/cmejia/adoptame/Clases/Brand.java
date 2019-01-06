package com.cmejia.adoptame.Clases;

public class Brand {
    private String descripcion;
    private int image;

    public Brand()
    {
        // Constructor vacio
    }

    public Brand(String description, int image)
    {
        this.descripcion = description;
        this.image = image;

    }

    public String getDescripcion()
    {
        return this.descripcion;
    }

    public int getImage()
    {
        return this.image;
    }

    public void setDescripcion(String descripcion)
    {
        this.descripcion = descripcion;
    }

    public void setImage(int image)
    {
        this.image = image;
    }
}
