struct genbu_transfer {
   /* Base class. */
   struct pipe_transfer b;
   struct pipe_resource *staging_texture;
};
