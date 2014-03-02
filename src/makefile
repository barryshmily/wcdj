ALLDIRS = comm \
          svr 
          

include ./Inc.mk

all:
	cd ${PROJECT_HOME} || exit "$$?"
	@for i in ${ALLDIRS}; \
	do \
		cd ${PROJECT_HOME} && cd ${PROJECT_HOME}/src/$$i >/dev/null; \
		echo "Makeing $$i"; \
		make; \
		cd ${PROJECT_HOME}/src/ >/dev/null; \
		echo "Done"; \
	done

clean:
	cd ${PROJECT_HOME} || exit "$$?"
	@for i in ${ALLDIRS}; \
	do \
		cd ${PROJECT_HOME} && cd ${PROJECT_HOME}/src/$$i >/dev/null; \
		echo "Cleaning $$i"; \
		make clean; \
		cd ${PROJECT_HOME}/src/ >/dev/null; \
		echo "Done"; \
	done
					
	cd ${PROJECT_HOME}/bin     && ls | grep -v ".*.sh"  | xargs rm -f
	cd ${PROJECT_HOME}/package && ls | grep -v "README" | grep -v "tar.sh"| xargs rm -rf

	
install: all
	cd ${PROJECT_HOME} || exit "$$?"
	@for i in ${ALLDIRS}; \
	do \
		cd ${PROJECT_HOME} && cd ${PROJECT_HOME}/src/$$i >/dev/null; \
		echo "Installing $$i"; \
		make install; \
		cd ${PROJECT_HOME}/src/ >/dev/null; \
		echo "Done"; \
	done	
	
	@echo "make package......"
	@cd ${PROJECT_HOME}/package && sh ./tar.sh
	@echo "make package ok!"


