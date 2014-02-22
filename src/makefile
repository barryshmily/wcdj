ALLDIRS = svr

include ./Inc.mk

all:
	@for i in ${ALLDIRS}; \
	do \
		cd ${PROJECT_HOME}/;cd ${PROJECT_HOME}/src/$$i >/dev/null ; \
		echo "Makeing $$i"; \
		make; \
		cd ${PROJECT_HOME}/src/ >/dev/null ; \
		echo "Done"; \
	done

clean:
	@for i in ${ALLDIRS}; \
	do \
		cd ${PROJECT_HOME}/;cd ${PROJECT_HOME}/src/$$i >/dev/null; \
		echo "Cleaning $$i"; \
		make clean; \
		cd ${PROJECT_HOME}/src/ >/dev/null; \
		echo "Done"; \
	done

	#cd ${PROJECT_HOME}/bin            && rm * -f;
					
	
install: all
	echo ${PROJECT_HOME}
	@for i in ${ALLDIRS}; \
	do \
		cd ${PROJECT_HOME}/;cd ${PROJECT_HOME}/src/$$i >/dev/null ; \
		echo "Installing $$i"; \
		make install; \
		cd ${PROJECT_HOME}/src/ >/dev/null ; \
		echo "Done"; \
	done	
	echo "make package......";
	sh ./tar.sh ;
	echo "make package ok!"; 


