
clean:
	$(RM) sidewinder/_sidewinder.c
	$(RM) sidewinder/*.pyc
	bash env/bin/activate
	python ./setup.py install
