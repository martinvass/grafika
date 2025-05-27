CC       := clang
CFLAGS   := -Wall -Wextra -std=c99 -I./includes
LDFLAGS  := -framework OpenGL -framework GLUT -lm

SRCS     := main.c types.c mesh.c textures.c scene.c camera.c input.c render.c
OBJDIR   := build
OBJS     := $(SRCS:%.c=$(OBJDIR)/%.o)

.PHONY: all clean

all: $(OBJDIR) virtual_gallery

virtual_gallery: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf virtual_gallery $(OBJDIR)
