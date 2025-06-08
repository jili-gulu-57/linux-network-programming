code:code.c
	gcc -o $@ $^ -std=c99 #$@ 和 $^ 是自动变量,$@表示当前规则中的目标文件，即code，$^表示当前规则中的依赖文件，即code.c
.PHONY:clean
clean:
	rm -f code

