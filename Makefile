build-default:
	cmake -S . -B build && cmake --build build

zip:
	zip -r project.zip Makefile CMakeLists.txt *.pdf src delivery mdl

demo-p1-s1:
	./build/bin/engine ./delivery/phase1/scene1/config.xml

demo-solar-system:
	./build/bin/engine ./delivery/phase2/solar-system/config.xml

demo-dynamic-solar-system:
	./build/bin/engine ./delivery/phase3/solar-system/config.xml

demo-texture-solar-system:
	./build/bin/engine ./delivery/phase4/solar-system/config.xml

demo-lighting:
	./build/bin/engine ./delivery/phase4/demo/lighting.xml

demo-textures:
	./build/bin/engine ./delivery/phase4/demo/textures.xml

