2012-12-03 11:38:54
-------------------------------------------------------
bin               : programming
build             : makefile build shell etc ...
include           : third party head file
lib               : third party lib file
Makefile          : project makefile , 
readme.txt        : me
src               : your files , .c .cpp .h .hpp
test              : unit test files
tutorial_libezman : tutorial

make all for static library only
make dyn for Dynamic library only

Usage of library in linux(unix) platform��
	ʹ�� -l ѡ��ָ����̬��Ͷ�̬��ĸ�ʽ����һ���ģ�������ļ���Ϊlibabc.so����ô���� -labc ���ɡ�
	����ʱ��ȥ����������ļ����������ϵͳ�⣬��ô����Ҫ��������������·���������ַ�����һ�����ڲ������� -L ѡ��ָ�����ļ�����·�������Բ��ж�������磺gcc -L /home/ddd -L/home/ddd/lib�� ��һ�ַ������ڻ�������������LD_LIBRARY_PATH ��������Ķ�̬���ļ����ڵ�·�������������������Sorlarise��Linux�����������HP��UX�£���SHLIB_PATH��
	��������·��-L ���� ָ����������$LD_LIBRARY_PATH�� �ڱ�������ʱ����-lxxx ��Ӧ�ÿ����ҵ� libxxx.so(����) ����libxxx.a,��ֻ�農̬��, �Ǿ�ɾ����̬��,�Ѿ�̬��ŵ�����·����.

centos export LD_LIBRARY_PATH=/home/wujj/Dropbox/stock/trunk/code/libOdip/lib
