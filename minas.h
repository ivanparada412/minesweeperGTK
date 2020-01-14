/*
 *MINAS.H - header del programa minas.out
 *
 *contiene las estructuras que se utilizaran en el programa
 *asi como las variables que se necesitan en mas de una funcion
 *
 *
 */
#include <stdio.h>
#include <gtk/gtk.h>


#define BOMBA 8
#define VACIA 9
#define NO 0
#define SI 1
#define BANDERA 10
/*
 *estructura jugador para guardar a el top 5
 *de los mejores jugadores
 */
typedef struct jugador
{
  gchar nombre[30];
  gint minutos;
  gint segundos;
}JUGADOR;

/*
 *estructura jugada para guardar las jugadas que
 *realiza el jugador
 */
typedef struct jugada
{
  gint renglon;
  gint columna;

}JUGADA;
/*
 *estructura botones para asocias varias variables a cada boton
 *y asi tener informacion de este
 */
typedef struct botones
{
  GtkWidget *boton;
  gint tipo;
  gint cerca;
  gint desplegada;
  gint bandera;
}BOTONES;

/*
 *estructua registro para guardar
 *una partida, aqui esta toda la informacion necesaria
 *
 */
typedef struct registro
{
  gint bombas[10][1][2];
  gint banderas[10][1][2];
  gint no_banderas;
  gint horas;
  gint minutos;
  gint segundos;
  gint no_jugadas;
}REGISTROJ;

/*
 *estructura elementos para pasar mas de una variable
 *a las funciones 
 */
typedef struct variables
{
  BOTONES *casilla[9][9];
  GtkWidget *imagen[10];
  REGISTROJ *guardar;
  gchar *titulo_guardar;
  gchar *titulo_abrir;
  gint descubiertas;
  gint flag_busca;
  GList *inicio, *mejores, *lista;
  // ventana principal, botones de la barra de herramientas para poder hacer acciones
  GtkWidget * label_time, *tabla, *barra_deshacer, *barra_rehacer, *deshacer_item, *rehacer_item, *ventana, *entry,  *ventana_gano;
 
  GtkWidget *ventana_perdio;
  //elementos de la ventana de presentacion 
  
  GtkWidget* ventana_pres;
}ELEMENTOS;
