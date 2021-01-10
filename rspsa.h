#ifndef RSPSA_H
#define RSPSA_H


#include <iostream>
#include <fstream>
#include <string> 
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <random>


using namespace std;


int nmaq;
int ndoc;
int npac;
int pacr;
int pacp;
int pacu;
int instancia;


vector<vector<int>> repDoc;
vector<vector<int>> repMaq;
vector<vector<int>> turnos;
vector<vector<int>> horarios_docs;
vector<vector<int>> docs_detalle;
vector<int> maquinas;
vector<vector<int>> maq_detalle;
vector<int>pacientes;
//vector<vector<int>> pacientes2;
vector<vector<int>> agendaP;
vector<vector<int>> doctoresP;
vector<vector<int>> maquinasP;
vector<int> listaP;

void iniciar(int npac, int ur, int pal, int rad, vector<int> &pacientes, vector<vector<int>> &agendaP, vector<vector<int>> &doctoresP, vector<vector<int>> &maquinasP, vector<vector<int>> detalleD, vector<vector<int>> detalleM);
vector<vector<vector<int>>> generar_vecino(int npac, int ur, int pal, int rad, vector<int> listaP, vector<vector<int>> d_detalle, vector<vector<int>> m_detalle);
vector<double> tiempoEsperaTotal(int ur, int pal, int rad, vector<int> pacientes, vector<vector<int>> agendaP);
bool comparacion(int &actual, int nueva, double temp);
void algoritmoSA(int npac, int ur, int pal, int rad, vector<int> &pacientes, vector<int> listaP, vector<vector<int>> &agendaP, vector<vector<int>> &doctoresP, vector<vector<int>> &maquinasP);
void transcribirSolFinal(vector<vector<int>> agendaP, vector<vector<int>> doctoresP, vector<vector<int>> maquinasP, vector<vector<int>> &repDoc, vector<vector<int>> &repMaq);
void escribir_resultados(vector<vector<int>> docs, vector<vector<int>> maqs);

#endif