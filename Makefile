INCDIR = ./include
SRCDIR = ./src
OBJDIR = ./obj
CC     = gcc
CFLAGS = -O2

%: $(OBJDIR)/%.o $(OBJDIR)/draw_tools.o $(OBJDIR)/libmnbbmp.a
	$(CC) $(CFLAGS) -o $@ $< $(OBJDIR)/draw_tools.o -L$(OBJDIR) -lmnbbmp -lpthread -lm

$(OBJDIR)/libmnbbmp.a: $(OBJDIR)/mnb_bmp.o $(OBJDIR)/led_matrix_c_sim.o
	ar rcs $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -I$(INCDIR) -o $@ -c $<

.PHONY: clean
clean:
	rm obj/*