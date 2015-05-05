
compile: xlainh00

.PHONY: clean pack test md5sumtest paritytest compile

VPATH=src
DEPS = $(VPATH)/proj2.h
OBJECT = $(VPATH)/file1.o $(VPATH)/file2.o $(VPATH)/file3.o
CFLAGS=-I$(VPATH)
CURL=-I../curl/include/ -L../curl/lib/.libs/ -lcurl


$(VPATH)%.o:%.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< $(CURL)

xlainh00:$(OBJECT)
	$(CC) $(CFLAGS) -o $@ $^ $(CURL)

clean:
	rm -f $(VPATH)/*.o xlainh00
	rm -f file1 file2 hashresult
#rm -r doc/

pack: clean
	tar cvzf ../IVS\ 2.proj.tgz ../IVS\ 2.proj/

#-------------------------------------------------------TEST-------------------------------------------------------------
URL=http://images.complex.com/complex/image/upload/t_article_image/eovn9qimnkdrculqlzxc.jpg
test: compile md5sumtest paritytest 

md5sumtest:
	@wget -O file1 $(URL)
	@chmod 755 xlainh00
	@./xlainh00 $(URL) file2; \
	\
	md5sum file1 file2 | md5sum -c > hashresult; \
	\
	cond1="file1: OK"; cond2="file2: OK"; file1=$$(awk 'NR==1' hashresult); file2=$$(awk 'NR==2' hashresult); \
	if [ "$$cond1" = "$$file1" ] && [ "$$cond2" = "$$file2" ]; then \
		echo "Download OK"; \
	else \
		echo "Download FAILED"; \
	fi

paritytest:
	@par1=$$(./xlainh00 file1.par); par2=$$(./xlainh00 file2.par); \
	if [ "$$par1" = "1" ] && [ "$$par2" = "1" ]; then \
		echo "Parity OK"; \
	else \
		echo "Parity FAILED"; \
	fi

