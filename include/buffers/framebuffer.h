#ifndef FRAMEBUFFER_H
# define FRAMEBUFFER_H

typedef struct s_framebuffer {
    int				width;
    int				height;
    unsigned char	*pixels;
}	t_framebuffer;

t_framebuffer	*framebuffer_allocate(int width, int height);
void			framebuffer_free(t_framebuffer *framebuffer);
void			framebuffer_clear(t_framebuffer *framebuffer, unsigned char color);
void			framebuffer_draw(t_framebuffer *framebuffer, int x, int y, unsigned char color);

#endif
