/*
 *
 *Modulo1.c- modulo complementario de el programa minas.out
 *
 *contiene las funciones , agrega_minas, es_mina, revisa, revisa_boton
 *bandera_boton, reiniciar y deshacer
 *
 *hace uso de la funcion imprime_ganador que no se encuentra declarada en este archivo
 */


#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "minas.h"



extern void imprime_ganador(gpointer info);
void revisa(gint cordx, gint cordy, gpointer info);
void revisa_boton(gpointer info);
//void busca(gint cordx, gint cordy, gpointer info);

/*
 *le asigna a los botones el tipo BOMBA
 *recibe, los numeros aleatorios hechos al iniciar
 *o las coordenadas de alguna partida guardada
 */
void agrega_minas(gint random1, gint random2,gpointer info, gint n)
{
  ELEMENTOS *variables = (ELEMENTOS*)info;

  /*
   *se asigna el tipo BOMBA y se guardan las coordenadas en un arreglo
   *por si se quiere guardar la partida
   */
  variables->casilla[random1][random2]->tipo = BOMBA;
  // g_print("%d %d\n", random1, random2);
  variables->guardar->bombas[n][0][0] = random1;
  variables->guardar->bombas[n][0][1] = random2;
}


/*
 *es_mina()- funcion asociada a la señas que emiten los botones
 *del tablero, revisa si le dio click a una mina y perdio
 *o si le dio a una mina vacia y debe desplegar mas
 *
 */
void es_mina(GtkWidget* widget, gpointer info)
{
  ELEMENTOS *variables;
  variables = (ELEMENTOS*)info;
  const gchar *coordenadas;
  gint cordx, cordy, cont, cont2, n = 0;
  GtkWidget* perdio[10];
  guint izq, der, arriba, abajo;
  JUGADA* nueva;
  GList* aux, *elim, *aux2, *elim2;
  nueva = malloc(sizeof(JUGADA));

  /*
   *se obtiene el nombre del widget, el cual son las coordenadas de este
   *se cambian a int y se verifica que tipo de boton es
   */
  coordenadas = gtk_widget_get_name(widget);
  sscanf(coordenadas, "%d-%d", &cordx, &cordy);
  // g_print("%d %d\n", cordx, cordy);
  if (variables->casilla[cordx][cordy]->tipo == BOMBA && variables->casilla[cordx][cordy]->bandera == NO)
    {

      /*
       *ciclo for para hacer 10 imagenes de mina
       *
       */
       for (cont = 0; cont < 10; cont++)
         {
           perdio[cont] = gtk_image_new_from_file("perdio.png");
         }

       /*
	*ciclo for para recorrer el tablero y desplegar las 10 minas que hay
	*si le dio click a una de estas
	* manda a llamar la funion de termina_juego
	*/
      for (cont = 0; cont < 9; cont++)
        {
          for (cont2 = 0; cont2 < 9; cont2++)
            {
              if (variables->casilla[cont][cont2]->tipo == BOMBA)
                {
                  gtk_widget_set_sensitive(variables->casilla[cont][cont2]->boton, FALSE);
                  gtk_button_set_image(GTK_BUTTON(variables->casilla[cont][cont2]->boton), perdio[n]);
                  n++;
                }
            }
        }
          termina_juego(info);
    }

  /*
   *si no le dio a una mina, se agregan las coordenadas de la jugada
   *en una lista, para poder tener el registro de jugadas
   *y se manda a revisar los botones de alrededor para ver si los 
   *despliega o no
   */
  else
    {
    
      //variables->inicio = g_list_append(variables->inicio, nueva);
      nueva->renglon = cordx;
      nueva->columna = cordy;

      /*
       *verifiacion por si habia dado deshacer
       *borrar las jugadas que deshizo y agregar
       *la que acaba de hacer
       *
       */
      if (g_list_next(variables->lista) != NULL)
	{
	  aux2 = g_list_next(variables->lista);
	  while(aux2 != NULL)
	    {
	      elim2 = g_list_next(aux2);
	      variables->inicio = g_list_delete_link(variables->inicio, aux2);
	      aux2 = elim2;
	      g_print("error");
	    }
	}
      /*
       *si le dio deshacer hasta llegar al inicio del tablero
       *el programa olvida todas las jugada y empieza de nuevo
       *
       */
      if (variables->guardar->no_jugadas == 0)
	{
	  variables->inicio = NULL;
	}

      /*
       *se agrega la jugada a la lista y se guarda cual esla jugada actual
       */
      variables->inicio = g_list_append(variables->inicio, nueva);
      variables->lista = g_list_last(variables->inicio);
      revisa(cordx, cordy, info);
      	gtk_widget_set_sensitive(variables->barra_deshacer, TRUE);
	gtk_widget_set_sensitive(variables->deshacer_item, TRUE);
	variables->guardar->no_jugadas++;

	/*
	 *si ya descubrio los 71 botones vacios, ha ganado y se despliega un mensaje
	 *
	 */
      if (variables->descubiertas == 71)
        {
          imprime_ganador(info);
	  g_print("gano");
        }
    }

}// es mina


/*
 *revisa()-funcion recursiva que verifica si se debe desplegar
 *los botones de alrededor, o solo desplegar el numero de minas que tiene cerca
 *
 */
void revisa(gint cordx, gint cordy, gpointer info)
{
  /*
   *se delcaran las variables y widgets necearios para modificar el tablero
   */
  ELEMENTOS* variables = (ELEMENTOS*)info;
  GtkWidget *imagen, *blanco;
  gchar *extension = ".png";
  gchar numero[7];

  blanco = gtk_image_new_from_file("blanco.png");
  /*
   *si este boton aun no se ha desplegado y no es bomba y no tiene bandera
   *entonces revisalo
   *
   */
  if ((variables->casilla[cordx][cordy]->desplegada == NO) && (variables->casilla[cordx][cordy]->tipo != BOMBA) && (variables->casilla[cordx][cordy]->bandera == NO))
    {
      /*
       *flag que busca la jugada actual para otras funciones
       */
      /*if (variables->flag_busca == 0)
	{
	  busca(cordx, cordy, info);
	  }*/

      /*
       *se dice que el boton ya esta desplegado, que se ha descubierto un boton
       *mas
       */
      variables->casilla[cordx][cordy]->desplegada = SI;
      variables->descubiertas++;
      g_print("%d descu\n", variables->descubiertas);

      /*
       *si tiene minas cerca, solo despliega la imagen
       *con el numero de minas que tiene
       *
       */
      if(variables->casilla[cordx][cordy]->cerca != 0)
        {
          //g_print("%d\n", variables->casilla[cordx][cordy]->cerca);
          sprintf(numero, "%d", variables->casilla[cordx][cordy]->cerca);
          strcat(numero, extension);
          //  g_print("%s\n", numero);
          imagen = gtk_image_new_from_file(numero);
          //gtk_widget_set_sensitive(variables->casilla[cordx][cordy]->boton, FALSE);
          gtk_button_set_image(GTK_BUTTON(variables->casilla[cordx][cordy]->boton), imagen);
        }

      /*
       *si no tiene minas cerca entonces se manda a revisar los botones
       *colindantes, verificando los limites del tabler, volviendo a llamar
       *esta misma funcion
       *
       */
      else
        {
	 
         
          gtk_widget_set_sensitive(variables->casilla[cordx][cordy]->boton, FALSE);
          gtk_button_set_image(GTK_BUTTON(variables->casilla[cordx][cordy]->boton), blanco);
          //g_print("si entra\n");
          if (cordy > 0)
            {
              revisa(cordx, cordy-1, info);
             
              if (cordx > 0)
                {
                  revisa(cordx-1, cordy-1, info);
                }

              if (cordx < 8)
                {
                  revisa(cordx + 1, cordy - 1, info);
                }
              
            }

          if (cordy < 8)
            {
              revisa(cordx, cordy + 1, info);

              if (cordx > 0)
                {
                  revisa(cordx - 1, cordy + 1, info);
                }

              if (cordx < 8)
                {
                  revisa(cordx + 1, cordy + 1, info);
                }
            }

          if (cordx > 0)
            {
              revisa(cordx - 1, cordy, info);
            }

          if (cordx < 8)
            {
              revisa(cordx + 1, cordy, info);
            }
        }
    }

}

/*
 *revisa_boton()-funcion que cuenta cuantas minas
 *tiene cerca cada boton
 * una vez que se han puesto las minas
 *
 */
void revisa_boton(gpointer info)
{
  ELEMENTOS* variables = (ELEMENTOS*)info;
  gint renglon_arriba, renglon_abajo, columna_izq, columna_der, cont, cont2;


  /*
   *doble ciclo que recorre el tablero haciendo las verificaciones
   *de los limites del tablero
   *si se encuentra hasta arriba, no verifica mas arriba etc
   *
   */
  for (cont = 0; cont < 9; cont++)
    {
      renglon_arriba = cont - 1;
      renglon_abajo = cont + 1;

      for (cont2 = 0; cont2 < 9; cont2++)
        {
          columna_izq = cont2 - 1;
          columna_der = cont2 + 1;

          if (renglon_arriba >= 0)
            {
              if (columna_izq >= 0)
                {
                  if (variables->casilla[renglon_arriba][columna_izq]->tipo == BOMBA)
                    {
                      variables->casilla[cont][cont2]->cerca++;
                    }
                }

              if (columna_der < 9)
                {
                  if (variables->casilla[renglon_arriba][columna_der]->tipo == BOMBA)
                    {
                      variables->casilla[cont][cont2]->cerca++;
                    }
                }

              if (variables->casilla[renglon_arriba][cont2]->tipo == BOMBA)
                {
                  variables->casilla[cont][cont2]->cerca++;
                }  
            }

          if (renglon_abajo < 9)
            {
              if (variables->casilla[renglon_abajo][cont2]->tipo == BOMBA)
                {
                  variables->casilla[cont][cont2]->cerca++;
                }

              if (columna_izq >= 0)
                {
                  if (variables->casilla[renglon_abajo][columna_izq]->tipo == BOMBA)
                    {
                      variables->casilla[cont][cont2]->cerca++;
                    }
                }

              if (columna_der < 9)
                {
                  if (variables->casilla[renglon_abajo][columna_der]->tipo == BOMBA)
                    {
                      variables->casilla[cont][cont2]->cerca++;
                    }
                  
                }
            }

          if (columna_izq >= 0)
            {
              if (variables->casilla[cont][columna_izq]->tipo == BOMBA)
                {
                  variables->casilla[cont][cont2]->cerca++;
                }
            }

          if (columna_der < 9)
            {
              if (variables->casilla[cont][columna_der]->tipo == BOMBA)
              {
                variables->casilla[cont][cont2]->cerca++;
              }
            }

          //  g_printf("cerca: %d de la %d %d\n", variables->casilla[cont][cont2]->cerca, cont, cont2);
        }

    }
}


/*
 *bandera_boton()-funcion asociada al click derecho 
 *coloca una bandera en el boton
 *y le da el tipo BANDER el cual evita que pueda ser accionado
 *si ya tiene bandera se la quita
 */
gboolean bandera_boton(GtkWidget *widget, GdkEvent* event, gpointer info)
{
  ELEMENTOS* variables;
  GtkWidget* imagen;
  const gchar* coordenadas;
  gint renglon, columna;

  coordenadas = gtk_widget_get_name(widget);
  variables = (ELEMENTOS*)info;
  imagen = gtk_image_new_from_file("flag.png");
  sscanf(coordenadas, "%d-%d", &renglon, &columna);

  /*
   *verifica que aun no se haya deplegado ese boton y que no tenga bandera para ponersela
   */
  if (event->button.button != 1 && variables->casilla[renglon][columna]->desplegada == NO && variables->casilla[renglon][columna]->bandera == NO )
    {
      gtk_button_set_image(GTK_BUTTON(variables->casilla[renglon][columna]->boton), imagen);
       variables->casilla[renglon][columna]->bandera = SI;
       return TRUE;
    }
  /*
   *verifia que ese boton no este desplegado y si ya tiene bandera
   *quitarsela
   */
  else if (event->button.button != 1 && variables->casilla[renglon][columna]->desplegada == NO && variables->casilla[renglon][columna]->bandera == SI)
    {
      variables->casilla[renglon][columna]->bandera = NO;
      gtk_button_set_image(GTK_BUTTON(variables->casilla[renglon][columna]->boton), NULL);
      
      return TRUE;
    }
  return FALSE;

} 

/*
 *reiniciar()-funcion asociada al boton de nuevo en la barra y el menu
 *reinicia el tablero, vuelve a generar minas y las agrea
 *
 */
void reiniciar(GtkWidget* widget, gpointer info)
{
  ELEMENTOS *variables = (ELEMENTOS*)info;
  gint cont, cont2, n, random1, random2;
  n = 0;

  /*
   *ciclo for doble para regresar toda la informacion de 
   *los botones a como estaba en el inicio
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
   *las variables de tiempo jugadas y listas
   *se vuelven a inicalizar
   *
   */
  variables->guardar->horas = 0;
  variables->guardar->minutos = 0;
  variables->guardar->segundos = 0;
  variables->guardar->no_jugadas = 0;
  variables->descubiertas = 0;
  variables->flag_busca = 0;
  variables->inicio = NULL;
  variables->lista = NULL;
  /* variables->actual = NULL;
  variables->rehacer = NULL;
  variables->guardar_a = NULL;*/


  /*
   *se generan otras 10 bombas aleatorias
   */
  while (n < 10)
    {
      random1 = rand() % 9;
      random2 = rand() % 9;

      if (variables->casilla[random1][random2]->tipo == -1)
        {
          agrega_minas(random1, random2, info, n);
          n++;
        }
    }
  /*
   *a los botones restante se les da el tipo vacio
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
   *se vuelve a mandar revisar cada boton para ver donde quedaron las minas
   *
   */
  revisa_boton(info);
}

/*
 *deshacer()-funcion asociada al boton de deshacer en la barra de herramientas
 *y el menu, da la opcion de deshacer cada jugada realizada, si se llega a la primer jugada
 *y se vuelve a dar deshacer, el juego se olvida de todas las jugadas realizadas
 *
 */
void deshacer(GtkWidget *widget, gpointer info)
{
  ELEMENTOS* variables;
  JUGADA *des;
  GList* jugada;
  variables = (ELEMENTOS*)info;
  gint cont, cont2, cordx, cordy, flags = 0;
  GtkWidget *flag_img[20];


  /*
   *se reinicia todo el tablero para despues generar todas las jugadas
   *exceto la que se quiere deshacer
   *
   */
    for (cont = 0; cont < 9; cont++)
    {
      for (cont2 = 0; cont2 < 9; cont2++)
        {
          
           gtk_widget_set_sensitive(variables->casilla[cont][cont2]->boton, TRUE);
           gtk_button_set_image(GTK_BUTTON(variables->casilla[cont][cont2]->boton), NULL);
	   if (variables->casilla[cont][cont2]->bandera == SI)
	     {
	       flag_img[flags] = gtk_image_new_from_file("flag.png");
	       gtk_button_set_image(GTK_BUTTON(variables->casilla[cont][cont2]->boton), flag_img[flags]);
	       flags++;
	     }
           variables->casilla[cont][cont2]->desplegada = NO;
           //variables->casilla[cont][cont2]->bandera = NO;
        }
    }

    /*
     *se verifica que haya jugada anterior
     *se recorre la lista
     *cuando llega a la jugada que se quiere deshacer
     *se sale del ciclo
     */
    variables->descubiertas = 0;
    variables->lista =g_list_previous(variables->lista); 
    if (variables->lista != NULL)
      {
	//variables->descubiertas = 0;
	jugada = g_list_first(variables->inicio);

	while(1)
	  {
	    des = (JUGADA*)jugada->data;
	    cordx = des->renglon;
	    cordy = des->columna;
	    revisa(cordx, cordy, info);
	    if (jugada == variables->lista)
	      {
		break;
	      }
	    jugada = g_list_next(jugada);   
	  }
      }
    /*
    if (variables->actual == NULL)
      {
	variables->actual =  g_list_previous(g_list_last(variables->inicio));
	variables->rehacer = g_list_last(variables->inicio);
      }
    
    else
      {
	variables->rehacer = variables->actual;
	variables->actual = g_list_previous(variables->actual);
      }
  
    if(variables->actual != NULL)
      {
	jugada = g_list_first(variables->inicio);

	while(jugada != g_list_next(variables->actual))
	  {
	    des = (JUGADA*)jugada->data;
	    cordx = des->renglon;
	    cordy = des->columna;
	    revisa(cordx, cordy, info);
	    jugada = g_list_next(jugada);
	  }
      }*/
    /*
     *se resta el numero de jugadas y descubiertas para no sumar de mas
     */
    variables->guardar->no_jugadas--;
    // variables->descubiertas = 0;
    /*
     *si no hay jugadas, desactiva boton
     */
    if (variables->guardar->no_jugadas == 0)
      {
	gtk_widget_set_sensitive(variables->barra_deshacer, FALSE);
	gtk_widget_set_sensitive(variables->deshacer_item, FALSE);
      }
}


/*
 *busca()
 *
 */
/*void busca(gint cordx, gint cordy, gpointer info)
{
   JUGADA* buscando;
  GList* aux;
  ELEMENTOS* variables = (ELEMENTOS*)info;
  aux = g_list_first(variables->inicio);


      while(aux != NULL)
        {
          buscando = (JUGADA*)aux->data;
          if(buscando->renglon == cordx && buscando->columna == cordy)
            {
	      g_print("%d %d guardar_a", cordx, cordy);
              variables->guardar_a = aux;
            }
	  g_print("%d %d coordenada\n", buscando->renglon, buscando->columna);
	    aux = g_list_next(aux);
        }
    

	}*/
