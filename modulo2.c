/*
 *Modulo2.c- modulo complementario para el programa minas.out
 *
 *contiene las funciones - rehacer, termina_juego, guardar, recuperar, imprime_ganador
 *registro, ordena_jugadores e imprime_jugadores
 *
 *hace uso de las funciones agrega_minas, reiniciar, revisa y revisa_boton declaradas en modulo1.c
 *
 *
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#include "minas.h"


extern void reiniciar(GtkWidget *widget, gpointer info);
//void escribe_archivo(GtkWidget *widget, gpointer info);
extern void agrega_minas(gint random1, gint random2, gpointer info, gint n);
extern void revisa(gint cordx, gint cordy, gpointer info);
extern void revisa_boton(gpointer info);
void registro(GtkWidget* widget, gpointer info);
gint ordena_jugadores(gconstpointer a, gconstpointer b);
void imprime_ganadores(gpointer info);

/*
 *rehacer()- funcion asociada al boton rehacer de la barra de herramientas y el menu
 *rehace la jugada anteriormente deshecha por la opcion de deshacer
 *
 */
void rehacer(GtkWidget* widget, gpointer info)
{
  ELEMENTOS* variables;
  JUGADA *rehacerj;
  GList *aux;
  gint renglon, columna;
  
  variables = (ELEMENTOS*)info;

  /*
   *se verifica que en la lista haya una jugada que rehacer, 
   *si es asi, le manda las coordenadas a la funcion revisa
   */
  if(g_list_next(variables->lista) != NULL)
    {
      variables->lista = g_list_next(variables->lista);
      rehacerj = (JUGADA*)variables->lista->data;
      renglon = rehacerj->renglon;
      columna = rehacerj->columna;
      revisa(renglon, columna, info);
      variables->guardar->no_jugadas++;
      gtk_widget_set_sensitive(variables->barra_deshacer, TRUE);
      gtk_widget_set_sensitive(variables->deshacer_item, TRUE);
    }
  /*
  if (variables->rehacer != NULL)
    {
      if (variables->rehacer == g_list_last(variables->inicio))
	{
	  variables->actual = NULL;
	}

      else
	{
	  variables->actual = variables->rehacer;
	}
      rehacerj = (JUGADA*)variables->rehacer->data;
      renglon = rehacerj->renglon;
      columna = rehacerj->columna;
      revisa(renglon, columna, info);
      variables->rehacer = g_list_next(variables->rehacer);
      variables->guardar->no_jugadas++;
      gtk_widget_set_sensitive(variables->barra_deshacer, TRUE);
      gtk_widget_set_sensitive(variables->deshacer_item, TRUE);
      }*/
}


/*
 *termina_juego()- funcion que se activa cuando el usuario ha perdido
 *se muestra una ventana de que perdio y se pregunta si se quiere volver a jugar
 *
 */
void termina_juego(gpointer info)
{
  ELEMENTOS* variables = (ELEMENTOS*)info;
  GtkWidget *boton_ok, *boton_cancelar, *label, *game_over;
  /*
   *se inicializan los widgets de la ventana
   *
   */
  game_over = gtk_image_new_from_file("gameover.png");
  boton_ok = gtk_button_new_with_label("Si");
  boton_cancelar = gtk_button_new_with_label("No");
  label = gtk_label_new("Has perdido!\n¿Quieres volver a jugar? ");
  variables->ventana_perdio = gtk_dialog_new();

  /*
   *se empaqueta todo y en la ventana de dialogo
   */
  gtk_window_set_title(GTK_WINDOW(variables->ventana_perdio), "Game over");
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(variables->ventana_perdio)->vbox), game_over, FALSE, FALSE, 10);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(variables->ventana_perdio)->vbox), label, FALSE, FALSE, 10);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(variables->ventana_perdio)->action_area), boton_ok, FALSE, FALSE, 10);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(variables->ventana_perdio)->action_area), boton_cancelar, FALSE, FALSE, 10);

  /*
   *se conectan las señales de los botones ok y cancelar y se muestra la ventana
   */
   g_signal_connect(G_OBJECT(boton_ok), "clicked", G_CALLBACK(reiniciar), info);
   g_signal_connect_swapped(G_OBJECT(boton_ok), "clicked", G_CALLBACK(gtk_widget_hide), variables->ventana_perdio);
   g_signal_connect_swapped(G_OBJECT(boton_cancelar), "clicked", G_CALLBACK(gtk_widget_hide), variables->ventana_perdio);
   gtk_widget_show_all(variables->ventana_perdio);
}

/*
 *guardar()-funcion asociada al boton guardar en la barra de herramientas y el menu
 *te permite guardar la partida actual en un archivo .bcm
 *
 */
void guardar(GtkWidget *widget, gpointer info)
{
  ELEMENTOS* variables = (ELEMENTOS*)info;
  GtkWidget *ventana_guardar, *ventana_error, *boton_aceptar, *label_error;
  gchar* filename;
  gint tam_file, cont, cont2;
  FILE* guardar;
  gboolean sobre;
  JUGADA* pos, *jugadas;
  GList*recorre;

  /*
   *se crea una ventana tipo file chooser dialog, que facilita el manejo de guardar el archivo
   *
   *
   */
  jugadas = malloc(sizeof(JUGADA));
  variables->guardar->no_banderas = 0;
  ventana_guardar = gtk_file_chooser_dialog_new("Guardar partida", GTK_WINDOW(variables->ventana), GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT, NULL);

  //gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(ventana_guardar), TRUE);
  
  /* if (user_edited_a_new_document)
    {
      gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(ventana_guardar), default_folder_for_saving);
      gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(ventna_guardar), "Partida sin titulo");
    }

  else
    {
      gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(ventana_guardar), filename_for_existing_document);
      }*/


  /*
   *si dio el boton aceptar se hacen verificaciones y se guarda
   *
   */
  if (gtk_dialog_run(GTK_DIALOG(ventana_guardar)) == GTK_RESPONSE_ACCEPT)
    {

      filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(ventana_guardar));
      tam_file = strlen(filename);

      /*
       *se verifia la extension correcta del archivo a guardar
       *
       */
      if (filename[tam_file - 1 ] == 'm' && filename[tam_file - 2] == 'c' && filename[tam_file - 3] == 'b' && filename[tam_file - 4] == '.')
      {
        
        gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(ventana_guardar), TRUE);
        sobre = gtk_file_chooser_get_do_overwrite_confirmation(GTK_FILE_CHOOSER(ventana_guardar));
	/*
	 *se pregunta si quiere sobreescribir un archivo en caso de existir
	 *
	 */
        if (sobre == TRUE)
          {
	    
            guardar = fopen(filename, "wb");
	    /*
	     *doble for para ver donde hay banderas y guardar su ubicacion
	     *
	     */
            for(cont = 0; cont < 9; cont++)
              {
                for (cont2 = 0; cont2 < 9; cont2++)
                  {
                    if(variables->casilla[cont][cont2]->bandera == SI)
                      {
                        variables->guardar->banderas[variables->guardar->no_banderas][0][0] = cont;
                        variables->guardar->banderas[variables->guardar->no_banderas][0][1] = cont2;
                        variables->guardar->no_banderas++; 
                      }
                  }

              }
	    /*   if (variables->actual != NULL)
              {
                pos = (JUGADA*)variables->actual->data;
                variables->guardar->actualg[0] = pos->renglon;
                variables->guardar->actualg[1] = pos->columna;
                g_print("escribo actual\n");
              }
            else
              {
                variables->guardar->actualg[0] = -1;
                variables->guardar->actualg[1] = -1;
              }
            if (variables->rehacer != NULL)
              {
                pos = (JUGADA*)variables->rehacer->data;
                variables->guardar->rehacerg[0] = pos->renglon;
                variables->guardar->rehacerg[1] = pos->columna;
                g_print("escribo rehacer\n");
              }
            else
              {
                variables->guardar->rehacerg[0] = -1;
                variables->guardar->rehacerg[1] = -1;
              }
            if (variables->guardar_a != NULL)
              {
                pos = (JUGADA*)variables->guardar_a->data;
                variables->guardar->posiciong[0] = pos->renglon;
                variables->guardar->posiciong[1] = pos->columna;
		}

            else
              {
               variables->guardar->posiciong[0] = -1;
               variables->guardar->posiciong[1] = -1; 
	       }*/
	    /*
	     *se escribe la estructura de guardar
	     *la cual contiene toda la informacion necesaria
	     *para reanudar el juego
	     */
            fwrite(variables->guardar, sizeof(REGISTROJ), 1, guardar);
	    /*
	     *se recorre la lista de jugadas 
	     *y cuando se llega a la actual deja de escribir
	     */
            recorre = g_list_first(variables->inicio);
            while(1)
              {
                jugadas = (JUGADA*)recorre->data;
                fwrite(jugadas, sizeof(JUGADA), 1, guardar);
                g_print("escribo jugadas\n");
		if(recorre == variables->lista)
		  {
		    break;
		  }
		 recorre = g_list_next(recorre);
              }
            fclose(guardar);
            g_print("quiere");
          }
        else
          {
            g_print("no quiere");
          }
        g_print("funciona");
      }

      /*
       *en caso de que la extension no sea valida se mostrara
       *un mensaje de error
       *
       */
      else
        {
          ventana_error = gtk_dialog_new();
          boton_aceptar = gtk_button_new_with_label("Aceptar");
          label_error = gtk_label_new("Error.\nLa extensión del archivo seleccionado\n no es válida");
          gtk_window_set_title(GTK_WINDOW(ventana_error), "Error ");
          //gtk_widget_set_usize(ventana_error, 500, 100);
          //gtk_widget_set_usize(boton_aceptar, 80, 80);
          gtk_box_pack_start(GTK_BOX(GTK_DIALOG(ventana_error)->vbox), label_error, FALSE, FALSE, 10);
          gtk_box_pack_start(GTK_BOX(GTK_DIALOG(ventana_error)->action_area), boton_aceptar, FALSE, FALSE, 10);
          g_signal_connect_swapped(G_OBJECT(boton_aceptar), "clicked", G_CALLBACK(gtk_widget_hide), ventana_error);
          gtk_widget_show_all(ventana_error);
        }
      g_free(filename);
    }
  gtk_widget_destroy(ventana_guardar);

}

/*
 *recuperar()- funcion asociada al boton recuperar en el menu juego
 *
 *recupera una partida guardada previamente en archivo .bcm
 *lee el archivo y muestra el juego como se quedo
 *
 *
 */
void recuperar(GtkWidget *widget, gpointer info)
{
  ELEMENTOS* variables = (ELEMENTOS*)info;
  REGISTROJ *partida_rec;
  GtkWidget *ventana_abrir, *ventana_error, *boton_aceptar, *label_error;
  GtkWidget *flags[10];
  gint tam_file, cont,cont2,  poner_banderas = 0, cordx, cordy, n = 0;
  FILE* abrir;
  JUGADA* anexa, *revisar;
  GList* aux, *sig;
  gpointer datos;

  /*
   *se crea una ventana tipo file chooser dialog la cual facilita el elegir un archivo
   *para poder abrir y leerlo
   *
   */
  partida_rec = malloc(sizeof(REGISTROJ));
  ventana_abrir = gtk_file_chooser_dialog_new("Abrir Partida", GTK_WINDOW(variables->ventana), GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,  GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);


  /*
   *si le dio aceptar 
   *se hacen verificaiones y se lee la partida
   *
   */
  if(gtk_dialog_run(GTK_DIALOG(ventana_abrir)) == GTK_RESPONSE_ACCEPT)
  {
    gchar* filename;
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(ventana_abrir));
    tam_file = strlen(filename);


    /*
     *si el archivo selecccionado es de la extension correcta
     *lo cargara
     */
     if (filename[tam_file - 1 ] == 'm' && filename[tam_file - 2] == 'c' && filename[tam_file - 3] == 'b' && filename[tam_file - 4] == '.')
      {
        abrir = fopen(filename, "rb");
        
        /*
	 *se reiniciar el tablero que pudiera estar en curso
	 *y despues de carga la partida guardada
	 *
	 */
        
          for (cont = 0; cont < 9; cont++)
            {
              for (cont2 = 0; cont2 < 9; cont2++)
                {
                  gtk_widget_set_sensitive(variables->casilla[cont][cont2]->boton, TRUE);
                  gtk_button_set_image(GTK_BUTTON(variables->casilla[cont][cont2]->boton), NULL);
                  variables->casilla[cont][cont2]->tipo = -1;
                  variables->casilla[cont][cont2]->cerca = 0;
                  variables->casilla[cont][cont2]->desplegada = NO;
                  variables->casilla[cont][cont2]->bandera = NO;
                }
            }

   
	  /*
	   *se verifica que se haya leido la estructura de guardar
	   *y se empieza a inicializr el juego
	   */
        if (fread(variables->guardar, sizeof(REGISTROJ), 1, abrir) == 1 )
          {
            for (cont = 0; cont < 10; cont++)
              {
                //g_print("%d prueba\n", variables->guardar->no_jugadas);
                agrega_minas(variables->guardar->bombas[cont][0][0], variables->guardar->bombas[cont][0][1], info, cont);
                //g_print("%d %d\n",variables->guardar->bombas[cont][0][0], variables->guardar->bombas[cont][0][1]);
                flags[cont] = gtk_image_new_from_file("flag.png");
              }
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
            revisa_boton(info);
	    /*
	     *se busca el numero de banderas que habia puesto en el juego
	     */
            while(n < variables->guardar->no_banderas)
              {
                // g_print("%d\n", variables->guardar->no_banderas);
                cordx = variables->guardar->banderas[n][0][0];
                cordy = variables->guardar->banderas[n][0][1];
                gtk_button_set_image(GTK_BUTTON(variables->casilla[cordx][cordy]->boton), flags[n]);
                n++;
              }

	    /*
	     *se inicializa la lista de jugadas a NULL
	     */
            variables->inicio = NULL;
	    variables->lista = NULL;
            //variables->actual = NULL;
            //variables->guardar_a = NULL;
            //variables->rehacer = NULL;
	    //variables->flag_busca = 1;
            anexa = malloc(sizeof(JUGADA));

	    /*
	     *se leen las jugadas del archivo
	     *y se van agregando a la lista
	     */
            while(fread(anexa, sizeof(JUGADA), 1, abrir) == 1)
              {
                g_print("%d %d", anexa->renglon, anexa->columna);
                variables->inicio = g_list_append(variables->inicio, anexa);
                g_print("leo jugadas\n\n");
                anexa = malloc(sizeof(JUGADA));
              }

	    /*if (variables->guardar->actualg[0] != -1)
              {
                aux = g_list_first(variables->inicio);
                  while(aux != NULL)
                    {
                      anexa = (JUGADA*)aux->data;
                      cordx = anexa->renglon;
                      cordy = anexa->columna;
                      if (variables->guardar->actualg[0] == cordx && variables->guardar->actualg[1] == cordy)
                        {
                          variables->actual = aux;
                        }

                      aux = g_list_next(aux);
                    }
                  g_print("leo apuntador actual\n");
              }
	     if (variables->guardar->rehacerg[0] != -1)
              {
                aux = g_list_first(variables->inicio);
                  while(aux != NULL)
                    {
                      anexa = (JUGADA*)aux->data;
                      cordx = anexa->renglon;
                      cordy = anexa->columna;
                      if (variables->guardar->rehacerg[0] == cordx && variables->guardar->rehacerg[1] == cordy)
                        {
                          variables->rehacer = aux;
                        }

                      aux = g_list_next(aux);
                    }
                  g_print("leo apuntador rehacer\n");
              }
            if (variables->guardar->posiciong[0] != -1)
              {
                aux = g_list_first(variables->inicio);
                  while(aux != NULL)
                    {
                      anexa = (JUGADA*)aux->data;
                      cordx = anexa->renglon;
                      cordy = anexa->columna;
		      
                      if (variables->guardar->posiciong[0] == cordx && variables->guardar->posiciong[1] == cordy)
                        {
			  g_print("%d %d de guardar", cordx, cordy);
                          variables->guardar_a = aux;
                        }

                      aux = g_list_next(aux);
                    }
                   g_print("leo apuntador rehacer\n");
                   }*/
	    /*
	     *una vez cargadas las jugadas se mandan a hacer estar al tablero
	     */
            aux = g_list_first(variables->inicio);
	    variables->lista = g_list_last(variables->inicio);
	    while(aux != NULL)
              {
	        
                anexa = (JUGADA*)aux->data;
                cordx = anexa->renglon;
                cordy = anexa->columna;
                g_print("%d %d\n", cordx, cordy);
                revisa(cordx, cordy, info);
                g_print("cambio tablero\n");	  
		aux = aux->next;
              }
          }
	variables->flag_busca= 0;
        fclose(abrir);
      }

     /*
      *si eligio un archivo con una extension no valida mostrara un mensaje de error
      *
      */
     else
      {
        ventana_error = gtk_dialog_new();
        boton_aceptar = gtk_button_new_with_label("Aceptar");
        label_error = gtk_label_new("Error.\nLa extensión del archivo seleccionado\n no es válida");
        gtk_window_set_title(GTK_WINDOW(ventana_error), "Error ");
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(ventana_error)->vbox), label_error, FALSE, FALSE, 10);
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(ventana_error)->action_area), boton_aceptar, FALSE, FALSE, 10);
        g_signal_connect_swapped(G_OBJECT(boton_aceptar), "clicked", G_CALLBACK(gtk_widget_hide), ventana_error);
        gtk_widget_show_all(ventana_error);
        }
   
     g_free(filename);
  }
  
  gtk_widget_destroy(ventana_abrir);


}

/*
 *imprime_ganador- funcion que se ejecuta cuando el usuario ha ganado 
 *el juego
 *
 */
void imprime_ganador(gpointer info)
{
  ELEMENTOS* variables = (ELEMENTOS*)info;
  FILE* fr;
  JUGADOR *recorre, *aux;
  GtkWidget *label_gano, *boton_ok, *caja;
  const gchar* nickname;
  gint tiempo_total, tiempo_actual;
  recorre = malloc(sizeof(JUGADOR));

  /*
   *se verifica que no haya aun jugadores en el score
   *si no hay automaticamente lo agrega
   */
  fr = fopen("registro.dat", "rb");
  if (fr == NULL)
    {
     
      variables->ventana_gano = gtk_window_new(GTK_WINDOW_TOPLEVEL);
      label_gano = gtk_label_new("Introduce tu nickname");
      variables->entry = gtk_entry_new();
      caja = gtk_vbox_new(TRUE, 10);
      boton_ok = gtk_button_new_with_label("OK");
      
      g_signal_connect(G_OBJECT(boton_ok), "clicked", G_CALLBACK(registro), info);
      
      gtk_box_pack_start_defaults(GTK_BOX(caja), label_gano);
      gtk_box_pack_start_defaults(GTK_BOX(caja), variables->entry);
      gtk_box_pack_start_defaults(GTK_BOX(caja), boton_ok);
      g_print("entro el if");
      gtk_container_add(GTK_CONTAINER(variables->ventana_gano), caja);

      gtk_widget_show_all(variables->ventana_gano);

    }

  /*
   *si si hay recorre la lista de jugadores 
   *y los agrega a una lista 
   */
  else
    {
      while(fread(recorre, sizeof(JUGADOR), 1, fr) == 1)
	{
	  variables->mejores = g_list_append(variables->mejores, recorre);
	  
	  /* tiempo_total = recorre->minutos *60;
	  tiempo_total += recorre->segundos;
	  tiempo_actual = variables->guardar->minutos * 60;
	  tiempo_actual += variables->guardar->segundos*/
	  // if(tiempo_actual < tiempo_total)
	    //  {
	  /*
	   *se muestra una ventana donde pide su nickname
	   *y despues se pasa a verificar si es de los 5 mejores
	   *
	   */
	     
	    recorre = malloc(sizeof(JUGADOR));
	    //  }
	    
	}
      variables->ventana_gano = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	      label_gano = gtk_label_new("Introduce tu nickname");
	      variables->entry = gtk_entry_new();
	      caja = gtk_vbox_new(TRUE, 10);
	      boton_ok = gtk_button_new_with_label("OK");
	      
	      g_signal_connect(G_OBJECT(boton_ok), "clicked", G_CALLBACK(registro), info);
	      
	      gtk_box_pack_start_defaults(GTK_BOX(caja), label_gano);
	      gtk_box_pack_start_defaults(GTK_BOX(caja), variables->entry);
	      gtk_box_pack_start_defaults(GTK_BOX(caja), boton_ok);
	      
	      gtk_container_add(GTK_CONTAINER(variables->ventana_gano), caja);
	      g_print("entro el else");
	      gtk_widget_show_all(variables->ventana_gano);
      fclose(fr);
    }
  
}


/*
 *registro()- funcion que carga al jugador actual
 *los ordena y los escribe en el archivo
 *
 */
void registro(GtkWidget* widget, gpointer info)
{
 const gchar *nombre;
  ELEMENTOS* variables = (ELEMENTOS*)info;
  FILE* fr;
  JUGADOR *mejor, *aux;
  GList* score;
  gint cinco = 0, tamanio, cont = 1;

  aux = malloc(sizeof(JUGADOR));
  mejor = malloc(sizeof(JUGADOR));
  /*
   *obtiene el nickname que el usuario entro
   *se guarda en una estructua junto con el tiempo que hizo
   */
  nombre = gtk_entry_get_text(GTK_ENTRY(variables->entry));
  //g_print("%s nombre", nombre);
  strcpy(mejor->nombre, nombre);
  mejor->minutos = variables->guardar->minutos;
  mejor->segundos = variables->guardar->segundos;
   variables->mejores = g_list_append(variables->mejores, mejor);
   //g_print("estoy enmedio");
  tamanio = g_list_length(variables->mejores);
  //g_print("%d tamanio", tamanio);
  /*
   *si hay mas de un jugador los ordena
   */
  if (tamanio > 1)
    {
      variables->mejores = g_list_sort(variables->mejores, ordena_jugadores);
    }
  //g_print("ya paso esta cosa");
  fr = fopen("registro.dat", "wb");
  score = g_list_first(variables->mejores);
  //g_print("llegamos a registro");
  /*
   *se recorre la lista y se escribe en el archivo
   */
  while ( cont <= tamanio)
    {
      g_print("score");
       aux = (JUGADOR*)score->data;
       fwrite(aux, sizeof(JUGADOR), 1, fr);
       score = g_list_next(score);
       cinco++;
       cont++;
       aux = malloc(sizeof(JUGADOR));

    }
  /*if (variables->mejores == NULL)
    {
      strcpy(mejor->nombre, nombre);
      mejor->minutos = variables->guardar->minutos;
      mejor->segundos = variables->guardar->segundos;
      fr = fopen("registro.dat", "wb");
      fwrite(mejor, sizeof(JUGADOR), 1, fr);
      fclose(fr);
    }

  else
    {
      fr = fopen("regist")


      }*/
  gtk_widget_destroy(variables->ventana_gano);
  fclose(fr);
  imprime_ganadores(info);
 
}

/*
 *ordena_jugadores()- funcion asociada a g_list_sort de los jugadores
 *los compara y regresa un valor segun se quiera el orden
 */
gint ordena_jugadores(gconstpointer a, gconstpointer b)
{
  JUGADOR *uno, *dos;
  gint tiempo1, tiempo2;
  if (a != NULL && b != NULL)
    {
      uno = (JUGADOR*)a;
      dos = (JUGADOR*)b;
      
      tiempo1 = uno->minutos * 60;
      tiempo1 += uno->segundos;
      tiempo2 = dos->minutos * 60;
      tiempo2 += dos->segundos;
      
      if(tiempo1 == tiempo2)
	{
	  return 0;
	}
      
      else if (tiempo1 > tiempo2)
	{
	  return 1;
	}
      
      else
	{
	  return -1;
	}
    }
  
}
/*
 *imprime_ganadores()- funcion que despliega a los 5 mejores jugadores
 *con el mejor tiempo
 *
 */
void imprime_ganadores(gpointer info)
{
  GtkWidget *ventana_jugadores, *label_1, *boton_score, *trofeo, *label_2, *label_3, *label_4, *label_5;
  FILE *fr;
  JUGADOR* aux, *jugador;
  gchar nombre[5][40];
  gint cont = 0;
  gint minutos[5];
  gint segundos[5];
  gchar com[5][100];
  /*
   *se inicializan los widgets de la ventana
   *
   */
  //g_print("entramos a imprime");
  /*
   *se abre el archivo de registro y se lee
   *
   */
  fr = fopen("registro.dat", "rb");
  ventana_jugadores = gtk_dialog_new();
  trofeo = gtk_image_new_from_file("trofeo.png");
  boton_score = gtk_button_new_with_label("Aceptar");

  /*
   *se recorre la lista y se leen solo los primeros 5 mejores
   *
   */
  aux = malloc(sizeof(JUGADOR));
    for (cont = 0; cont < 5; cont++)
    {
      sprintf(com[cont], "%d-", cont + 1);
    }
    cont = 0;
  while(fread(aux, sizeof(JUGADOR), 1, fr) == 1)
    {
     
      strcpy(nombre[cont], aux->nombre);
      minutos[cont] = aux->minutos;
      segundos[cont] = aux->segundos;
      sprintf(com[cont], "%d- %s- %d min, %d seg", cont +1, nombre[cont], minutos[cont], segundos[cont]);
      g_print("%s", com[cont]);
      cont++;
      /*
       *si ya hay 5 se sale del ciclo
       */
      if(cont == 5)
	{
	  break;
	}
      aux = malloc(sizeof(JUGADOR));
    }

  /*
   *se agregan los nombres a los labels
   *
   */
    label_1 = gtk_label_new(com[0]);
    label_2 = gtk_label_new(com[1]);
    label_3 = gtk_label_new(com[2]);
    label_4 = gtk_label_new(com[3]);
    label_5 = gtk_label_new(com[4]);

    /*
     *se junta todo en la ventana de dialogo 
     *
     */
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(ventana_jugadores)->vbox), trofeo, FALSE, FALSE, 10);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(ventana_jugadores)->vbox), label_1, FALSE, FALSE, 10);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(ventana_jugadores)->vbox), label_2, FALSE, FALSE, 10);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(ventana_jugadores)->vbox), label_3, FALSE, FALSE, 10);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(ventana_jugadores)->vbox), label_4, FALSE, FALSE, 10);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(ventana_jugadores)->vbox), label_5, FALSE, FALSE, 10);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(ventana_jugadores)->action_area), boton_score, FALSE, FALSE, 10);
 
  /*
   *se conecta la señal del boton ok de la venta y se muestra
   *
   */
  g_signal_connect_swapped(G_OBJECT(boton_score), "clicked", G_CALLBACK(gtk_widget_hide), ventana_jugadores);
  //g_signal_connect_swapped(G_OBJECT(boton_cancelar), "clicked", G_CALLBACK(gtk_widget_hide), variables->ventana_perdio);
   gtk_widget_show_all(ventana_jugadores);
}
