Alumnos:
Ivan Spalla Ugalde
Marc Nadal Sastre Gondar
Veimar Israel Flores Rios


Observaciones:
Cuando se crean procesos en segundo plano con el minishell, aunque se cierre el minishell estos procesos aun siguen ejecutandose. 
Por tanto, hemos implementado una funcion jobs_purge() en el nivel6.c y my_shell.c para que cuando se reciba el comando "exit"
se recorra jobs_list eliminando todos los procesos y haciendole un kill(pid, SIGKILL) a cada uno.
Ademas hay una variable de DEBUG llamada DEBUGMINE usada para comprobar el correcto funcionamiento de algunas funciones, pero que
no daban informacion que se pidiera en los documentos, asi que hemos preferido crear una variable auxiliar para nuestros test de prueba
