/*
 *MINAS- EL JUEGO MAS ADICTIVO!!!!!
 *
 *AUTORES - ANDREA BUEYES / IVÁN PARADA
 *
 *FECHA - 21 DE MAYO 2015
 *
 *PROGRAMACIÓN APLICADA
 *
 *PROYECTO FINAL
 *
 *USO: ./minas.out las reglas generales se encuentran la ventana ayuda
 *
 *MODULOS EXTERNOS- modulo1.c modulo2.c
 *
 *
 */

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "minas.h"

/*
 *funciones que manejan el cerrar las ventanas de ayuda y acerca
 */
gboolean delete_event_handler(GtkWidget *widget, GdkEvent *event, gpointer info);
gboolean delete_event_handler_ayuda(GtkWidget *widget, GdkEvent *event, gpointer info);
gboolean delete_event_handler_acerca(GtkWidget *widget, GdkEvent *event, gpointer info);
/*
 *las funciones se encuentran en el archivo minas.c, modulo1.c y modulo2.c
 *las especificaciones se encuentran en cada una
 */
extern void imprime_ganador(gpointer info);
void presentar(ELEMENTOS* variables);//*
void ok_presentar(GtkWidget* widget, gpointer info);//*
void iniciar(gpointer info);//*
extern void reiniciar(GtkWidget *widget, gpointer info);//*
extern void guardar(GtkWidget *widget, gpointer info);//*
extern void recuperar(GtkWidget *widget, gpointer info);
extern void deshacer(GtkWidget *widget, gpointer info);//*
extern void rehacer(GtkWidget *widget, gpointer info);//*
void ayuda(GtkWidget* widget, gpointer info);
void acerca_de(GtkWidget* widget, gpointer info);
void terminar(GtkWidget *widget, gpointer info);
extern void revisa(gint cordx, gint cordy, gpointer info);//*
extern void revisa_boton(gpointer info);//*
extern void agrega_minas(gint random1, gint random2, gpointer info, gint n);//*
int incrementa(gpointer info);//*
void es_mina(GtkWidget* widget, gpointer info);//*
void cerrar(void);
void estado_boton(GtkWidget *widget, GtkWidget* ventana);//*
extern void termina_juego(gpointer info);//*
extern gboolean bandera_boton(GtkWidget* widget, GdkEvent *event, gpointer info);//*


int main(int argc, char** argv)
{
  ELEMENTOS* variables;// estructura global para el paso de variables a funciones
  gtk_init(&argc, &argv);
  variables = malloc(sizeof(ELEMENTOS)); // espacio para la estructura
  variables->guardar = malloc(sizeof(REGISTROJ)); // espacio para la estructura anidada
  //llamamos la funcion que despliega la funcion de presentar
  presentar(variables);


  gtk_main();

  return 0;
}
/*
 *presentar(); muestra la pantalla de presentacion y al dar click ok
 *muestra la pantalla de juego
 *
 *parametros - variables, contiene variables necesarias en otras funciones
 *
 *retorno-void
 */
void presentar(ELEMENTOS* variables)
{
  GtkWidget *caja_pres, *label_pres, *imagen_pres, *boton_pres, *caja_boton;
  /*
   *inicializacion de los widgets
   *
   */
  variables->ventana_pres = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  caja_pres = gtk_vbox_new(FALSE, 10);
  label_pres = gtk_label_new("BUSCA MINAS\n\nDesarrollado por: Andrea Bueyes e Iván Parada\n\n");
  boton_pres = gtk_button_new_with_label("Empezar");
  imagen_pres = gtk_image_new_from_file("mina.png");
  caja_boton = gtk_hbutton_box_new();

  /*
   *definicion de parametros de la ventana
   */
  gtk_widget_set_usize(variables->ventana_pres, 750, 750);
  gtk_window_set_title(GTK_WINDOW(variables->ventana_pres), "Busca Minas");
  gtk_widget_set_size_request(boton_pres, 250, 50);

  /*
   *conect de la señal del boton, a la funcion ok_presentar
   */
  g_signal_connect(G_OBJECT(boton_pres), "clicked", G_CALLBACK(ok_presentar), variables);

  /*
   *se empaca todo en la caja, se pone en la ventana
   *y se muestra
   */
  gtk_box_pack_start_defaults(GTK_BOX(caja_pres), label_pres);
  gtk_box_pack_start_defaults(GTK_BOX(caja_pres), imagen_pres);
  gtk_box_pack_start_defaults(GTK_BOX(caja_pres), caja_boton);
  gtk_container_add(GTK_CONTAINER(caja_boton), boton_pres);
  gtk_container_add(GTK_CONTAINER(variables->ventana_pres),caja_pres);

  gtk_widget_show_all(variables->ventana_pres);

}

/*
 *
 *ok_presentar()- funcion asociada al boton ok de la pantalla de presentacion
 *
 *destruye la ventana de presentacion y manda a llamar la funcion que incializa la pantalla de juego
 *
 */
void ok_presentar(GtkWidget* widget, gpointer info)
{
  ELEMENTOS* variables = (ELEMENTOS*)info;
  gtk_widget_destroy(variables->ventana_pres);
  iniciar(info);
}

/*
 *iniciar()funcion que incializa pantallas y variables necesarias para todas las opciones del juego
 *
 *parametros- info contiene variables necesarias en esta y otras funciones
 *
 *
 *
 */
void iniciar(gpointer info)
{
  gint izq = 0;
  gint der = 0;
  gint abajo = 0;
  gint arriba = 0;
  gint renglon = 0;
  gint columna = 0;
  gint cont, cont2, randoms = 0, random1, random2;
  gchar nombre[2];
  gint tiempo, id;
  ELEMENTOS *variables = (ELEMENTOS*)info;
  /*
   *declaracion de widgets
   *
   */
  GtkWidget  *caja,
    *barra_menu, *juego_item, *menu_juego, *nuevo_item, *guardar_item, *recuperar_item, *terminar_item,
    *herramientas_item, *menu_herramientas, *mas_item, *menu_mas, *ayuda_item, *acerca_de_item, *barra_herramientas, *imagen_guardar, *imagen_nuevo, *imagen_deshacer, *imagen_rehacer, *barra_guardar, *barra_nuevo;

  GtkWidget *barra_estado;

  /*
   *
   *se inicalizan todos los widgets necesarios y se les dan atributos
   *algunos son locales, los que comienzan con variables-> se encuentran en la estructura
   *
   *
   */
  srand(time(0));
  variables->ventana = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  caja = gtk_vbox_new(FALSE, 3);
  barra_herramientas = gtk_toolbar_new();
  variables->tabla = gtk_table_new(9, 9, TRUE);
  /*
   *items necesarios para la barra de herramientas y el menu
   *
   */
  barra_menu = gtk_menu_bar_new();
  juego_item = gtk_menu_item_new_with_label("Juego");
  menu_juego = gtk_menu_new();
  nuevo_item = gtk_menu_item_new_with_label("Nuevo");
  guardar_item = gtk_menu_item_new_with_label("Guardar...");
  recuperar_item = gtk_menu_item_new_with_label("Recuperar...");
  terminar_item = gtk_menu_item_new_with_label("Terminar juego");
  herramientas_item = gtk_menu_item_new_with_label("Herramientas");
  menu_herramientas = gtk_menu_new();
  variables->rehacer_item = gtk_menu_item_new_with_label("Rehacer");
  variables->deshacer_item = gtk_menu_item_new_with_label("Deshacer");
  mas_item = gtk_menu_item_new_with_label("Más");
  menu_mas = gtk_menu_new();
  ayuda_item = gtk_menu_item_new_with_label("Ayuda");
  acerca_de_item = gtk_menu_item_new_with_label("Acerca de");

  
  /*
   *imagenes necesarias para la interfaz
   *
   */
  imagen_guardar = gtk_image_new_from_file("guardar.png");
  imagen_nuevo = gtk_image_new_from_file("nuevo.png");
  imagen_deshacer = gtk_image_new_from_file("deshacer.png");
  imagen_rehacer = gtk_image_new_from_file("rehacer.png");
  /*
   *funcion que llama el cronometro para contar el tiempo
   */
  id = gtk_timeout_add(1000, (GSourceFunc)incrementa, (gpointer)variables);
  barra_estado = gtk_statusbar_new();
  variables->label_time = gtk_label_new("");
 

  gtk_window_set_title(GTK_WINDOW(variables->ventana), "Busca minas!");
  gtk_widget_set_usize(variables->label_time, 30, 30);

  /*
   *inicializacion de las variables de juego en 0
   *y de las listas en NULL
   */
  variables->descubiertas = 0;
  variables->guardar->no_jugadas = 0;
  variables->guardar->segundos = 0;
  variables->guardar->minutos = 0;
  variables->guardar->horas = 0;
  variables->flag_busca = 0;
  variables->inicio = NULL;
  variables->actual = NULL;
  variables->rehacer = NULL;
  variables->lista = NULL;
  variables->mejores = NULL;

  /*
   *ciclo for para inicializar las variables asociadas a cada boton
   *y darle un tamaño
   *
   */
  for (cont = 0; cont < 9; cont++)
    {
      for (cont2 = 0; cont2 < 9; cont2++)
	{
	  sprintf(nombre, "%d-%d", cont, cont2);
	  variables->casilla[cont][cont2] = malloc(sizeof(BOTONES));
	  variables->casilla[cont][cont2]->boton = gtk_button_new();
	  variables->casilla[cont][cont2]->tipo = -1;
          variables->casilla[cont][cont2]->cerca = 0;
          variables->casilla[cont][cont2]->desplegada = NO;
          variables->casilla[cont][cont2]->bandera = NO;
	  gtk_widget_set_name(variables->casilla[cont][cont2]->boton, nombre);
          gtk_widget_set_usize(variables->casilla[cont][cont2]->boton, 70, 70);
	  
	}
    }


  /*
   *creacion aleatoria de las bombas
   *se verifica que no se pongan dos bombas
   *en un mismo lugar
   */
  while (randoms < 10)
    {
      random1 = rand() % 9;
      random2 = rand() % 9;

      if (variables->casilla[random1][random2]->tipo == -1)
	{
	  agrega_minas(random1, random2, info, randoms);
	  randoms++;
	}
    }

  /*
   *una vez puestas las bombas, se inicializan los demas
   *botones en VACIA
   *
   */
  for (cont = 0; cont < 9; cont++)
    {
      for (cont2 = 0; cont2 < 9; cont2++)
	{
	  if (variables->casilla[cont][cont2]->tipo == -1)
	    {
	      variables->casilla[cont][cont2]->tipo = VACIA;
	    }
	}
    }

  /*
   *mandamos a revisar la distribucion del tablero
   *para saber donde estan las bombas
   */
  revisa_boton(info);

  /*
   *se conectan las señales de todos los items y botones a su funcion correspondiente
   *
   */
  g_signal_connect(G_OBJECT(nuevo_item), "activate", G_CALLBACK(reiniciar), info);
  g_signal_connect(G_OBJECT(guardar_item), "activate", G_CALLBACK(guardar), info);
  g_signal_connect(G_OBJECT(recuperar_item), "activate", G_CALLBACK(recuperar), info);
  g_signal_connect(G_OBJECT(variables->deshacer_item), "activate", G_CALLBACK(deshacer), info);
  g_signal_connect(G_OBJECT(variables->rehacer_item), "activate", G_CALLBACK(rehacer), info);
  g_signal_connect(G_OBJECT(ayuda_item), "activate", G_CALLBACK(ayuda), info);
  g_signal_connect(G_OBJECT(acerca_de_item), "activate", G_CALLBACK(acerca_de), info);
  g_signal_connect(G_OBJECT(terminar_item), "activate", G_CALLBACK(terminar), info);
  g_signal_connect(G_OBJECT(variables->ventana), "delete_event", G_CALLBACK(delete_event_handler), NULL);
  g_signal_connect(G_OBJECT(variables->ventana), "destroy", G_CALLBACK(cerrar), NULL);
  g_signal_connect(G_OBJECT(terminar_item), "activate", G_CALLBACK(cerrar), NULL);

  /*
   *se hacer un for para conectar las señales de los botones
   */
  for (cont = 0; cont < 9; cont++)
    {
      for (cont2 = 0; cont2 < 9; cont2++)
	{
	  g_signal_connect(G_OBJECT(variables->casilla[cont][cont2]->boton), "clicked", G_CALLBACK(es_mina), info);
          g_signal_connect(G_OBJECT(variables->casilla[cont][cont2]->boton), "clicked", G_CALLBACK(estado_boton), barra_estado);
          g_signal_connect(G_OBJECT(variables->casilla[cont][cont2]->boton), "button-press-event", G_CALLBACK(bandera_boton), info);
	}
    }
  cont = 0;
  cont2 = 0;

  /*
   *ciclos for para agregar los botones a la tabla, esto se hace de izq a derecha y de arriba a abajo
   *
   */
  for (abajo = 1; abajo < 10; abajo++)
    {
      for (der = 1; der < 10; der++)
	{
	  gtk_table_attach_defaults(GTK_TABLE(variables->tabla), variables->casilla[cont][cont2]->boton, izq, der, arriba, abajo);
          izq++;
          cont2++;
	}
      arriba++;
      izq = 0;
      cont++;
      cont2 = 0;
    }
  /*
   *se agregan los items de la barra de herramientas a la barra
   */
  barra_guardar = gtk_toolbar_append_item(GTK_TOOLBAR(barra_herramientas), "Guardar", "guardar partida actual", "Private", imagen_guardar, GTK_SIGNAL_FUNC(guardar), variables);
  barra_nuevo = gtk_toolbar_append_item(GTK_TOOLBAR(barra_herramientas), "Nuevo", "Crear nueva partida", "Private", imagen_nuevo, GTK_SIGNAL_FUNC(reiniciar), variables);
  variables->barra_deshacer = gtk_toolbar_append_item(GTK_TOOLBAR(barra_herramientas), "Deshacer", "Deshacer ultima jugada", "Private", imagen_deshacer, GTK_SIGNAL_FUNC(deshacer), variables);
  variables->barra_rehacer = gtk_toolbar_append_item(GTK_TOOLBAR(barra_herramientas), "Rehacer", "Rehacer la última jugada", "Private", imagen_rehacer, GTK_SIGNAL_FUNC(rehacer), variables);

  
  /*
   *se agregan los item del menu al menu
   *y submenus
   */
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_juego), nuevo_item);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_juego), guardar_item);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_juego), recuperar_item);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_juego), terminar_item);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(juego_item), menu_juego);
  gtk_menu_bar_append(GTK_MENU_BAR(barra_menu), juego_item);
  
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_herramientas), variables->deshacer_item);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_herramientas), variables->rehacer_item);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(herramientas_item), menu_herramientas);
  gtk_menu_bar_append(GTK_MENU_BAR(barra_menu), herramientas_item);

  gtk_menu_shell_append(GTK_MENU_SHELL(menu_mas), ayuda_item);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_mas), acerca_de_item);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(mas_item), menu_mas);
  gtk_menu_bar_append(GTK_MENU_BAR(barra_menu), mas_item);


  /*
   *se mete todo lo necesario a la caja y esta a la ventana
   *se muestra la ventana y se empieza a jugar
   *
   */
  gtk_box_pack_start_defaults(GTK_BOX(caja), barra_menu);
  gtk_box_pack_start_defaults(GTK_BOX(caja), barra_herramientas);
  gtk_box_pack_start_defaults(GTK_BOX(caja), variables->label_time);
  gtk_box_pack_start_defaults(GTK_BOX(caja), variables->tabla);
  gtk_box_pack_start_defaults(GTK_BOX(caja), barra_estado);
 
  gtk_container_add(GTK_CONTAINER(variables->ventana), caja);
  gtk_widget_show_all(variables->ventana);
}

/*
 *ayuda()
 *funcion que despliega el funcionamiento general del programa y del juego
 *se despiega cuando se da click en el menu de mas, ayuda
 */
void ayuda(GtkWidget* widget, gpointer info)
{
  /*
   *se declaran los widgets necesarios para la ventana
   */
  GtkWidget *ayuda,  *etiqueta2, *caja2;
   ayuda = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   etiqueta2 = gtk_label_new(" Instrucciones:\n\n *En la Barra de Herramientas, se van a encontrar imagenes con algunas opciones como:\n  nuevo, guardar, rehacer y deshacer.\n *En la Barra de Estado, se encuentra el nombre del juego.\n Hay tres tipos de menús que son los siguientes:\n *Jugar:\n En este menú se encuentran las opciones: nuevo, guardar, terminar y recuperar.\n *Herramientas:\n En este menú se encuentran las opciones: rehacer y deshacer.\n *Otros:\n En este menú se encuentra: ayuda y acerca de...\n\n Reglas:\n\n 1.Dar clic en cualquier recuadro.\n 2. Los números representan la cantidad de minas que se encuentran adyacentes.\n 3. Hacer clic en recuadros que estás seguro que no tienen minas.\n 4. Haciendo clic derecho puedes marcar las minas.\n 5. Una vez encontradas todas las minas haz ganado. ");
    caja2 = gtk_hbox_new(TRUE, 2);

    /*
     *se le dan atributos a los widgets
     *
     */
    gtk_widget_set_usize(ayuda, 600, 600);
    gtk_window_set_title(GTK_WINDOW(ayuda), "Ayuda"); caja2 = gtk_hbox_new(TRUE, 2);
    gtk_window_set_position(GTK_WINDOW(ayuda), GTK_WIN_POS_MOUSE);
    /*
     *se conecta la señal de cerrar las ventanas
     *se empaca todo y se muestra la ventana
     */
    g_signal_connect(G_OBJECT(ayuda), "destroy", G_CALLBACK(gtk_widget_destroy), ayuda);
    g_signal_connect(G_OBJECT(ayuda), "delete_event", G_CALLBACK(delete_event_handler_acerca), ayuda);
    gtk_box_pack_start_defaults(GTK_BOX(caja2), etiqueta2);
    gtk_container_add(GTK_CONTAINER(ayuda), caja2);
    gtk_widget_show_all(ayuda);
}


/*
 *
 *acerca_de()funcion que despliega informacion de los desarrolladores del programa
 *esta se muestra dando click en el menu mas, acerca de
 *
 */
void acerca_de(GtkWidget* widget, gpointer info)
{
  /*
   *se declaran los widgets necesarios para la ventana
   */
 GtkWidget *acerca, *etiqueta1, *caja1;
 
 /*
  *se inicializan los widgets
  */
  acerca = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  etiqueta1 = gtk_label_new("Desarrollado por: \n Andrea Bueyes Roiz, 189313-1 \n Diego Iván Parada Baleón, 192847-2 ");
 
  caja1 = gtk_hbox_new(TRUE, 2);
 
  /*
   *se le da atributos a la ventana
   *
   */
  gtk_widget_set_usize(acerca, 600, 600);
  gtk_window_set_title(GTK_WINDOW(acerca), "Acerca de...");
  gtk_window_set_position(GTK_WINDOW(acerca), GTK_WIN_POS_CENTER);

  /*
   *se conectan las señales de cerrar la ventana y se muestra
   */
  g_signal_connect(G_OBJECT(acerca), "delete_event", G_CALLBACK(delete_event_handler_acerca), acerca);
  g_signal_connect(G_OBJECT(acerca), "destroy", G_CALLBACK(gtk_widget_destroy), acerca);
  gtk_box_pack_start_defaults(GTK_BOX(caja1), etiqueta1);
 
  gtk_container_add(GTK_CONTAINER(acerca), caja1);
  
  gtk_widget_show_all(acerca);
}
void terminar(GtkWidget *widget, gpointer info)
{

}
void cerrar(void)
{
  gtk_main_quit();
}

/*
 *incrementa()- funcion asociada al cronometro del juego
 *esta funcion se manda a llamar cada segundo y actualiza el tiempo transcurrido 
 *de juego
 *
 */
gint incrementa(gpointer info)
{
  ELEMENTOS* variables = (ELEMENTOS*)info;
  gchar tiempo[60];

  if (variables->guardar->no_jugadas > 0)
    {

      /*
       *si ya empezo a jugar, se empieza a incrementar el crónometro
       *y se van actualizando las horas minutos y segundos
       */
      variables->guardar->segundos++;
      if (variables->guardar->segundos == 60)
        {
          variables->guardar->minutos++;
          variables->guardar->segundos = 0;

          if (variables->guardar->minutos == 60)
            {
              variables->guardar->horas++;
              variables->guardar->minutos = 0;
            }
        }
      /*
       *se cambia el texto en el label del cronometro
       */
      sprintf(tiempo, "Horas: %d Minutos: %d Segundos: %d",variables->guardar->horas, variables->guardar->minutos, variables->guardar->segundos);
      gtk_label_set_text(GTK_LABEL(variables->label_time), tiempo);
    }

   return TRUE;
}


/*
 *estado_boton()- funcion que avisa cuando un boton de el tablero fue accionado en la barra de estado
 *
 */
void estado_boton(GtkWidget *widget, GtkWidget* ventana)
{

  gtk_statusbar_push(GTK_STATUSBAR(ventana), gtk_statusbar_get_context_id(GTK_STATUSBAR(ventana), "casilla accionada"), "casilla accionada");

}

/*
 *delete_event_handler() funciones para el manejo de cierre de ventanas
 */
gboolean delete_event_handler(GtkWidget *widget, GdkEvent *event, gpointer info)
{
  return FALSE;
}
gboolean delete_event_handler_ayuda(GtkWidget *widget, GdkEvent *event, gpointer info)
{
  return FALSE;
}
gboolean delete_event_handler_acerca(GtkWidget *widget, GdkEvent *event, gpointer info)
{
  return FALSE;
}
