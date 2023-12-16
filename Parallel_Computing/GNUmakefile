.SILENT:

.PHONY: test
test: test/ test/a.exe
	echo	Num-Threads	O-Time-per-File	I-Time	IO-Time
	cd test/ ; for num_threads in `seq 1 ${MY_NUM_THREADS}`; do  \
				 echo -n $$num_threads ' ' ;  \
				 for io_stat in {0..2}; do  \
				   echo 3 > /proc/sys/vm/drop_caches ;  \
				   OMP_NUM_THREADS=$$num_threads  \
					 ./a.exe $$io_stat $$MY_NUM_FILES $$MY_AMOUNT_CALC;  \
				   echo -n ' ' ;  \
				 done ;  \
				 echo ;  \
			   done

test/ :
	mkdir --parents  test/

test/a.exe : a.cpp test/
	g++ -std=c++20 -O0 -Wall  \
		-fopenmp  \
		-o test/a.exe  \
		a.cpp

.PHONY: clean
clean:
	rm --recursive --force  test/
	rm --force  --  *~  *\#*\#

# Local Variables:
# indent-tabs-mode: t
# tab-width: 4
# End:
