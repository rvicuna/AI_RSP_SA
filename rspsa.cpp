#include <iostream>
#include <fstream>
#include <string> 
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <random>

#include "rspsa.h"


//RADIOTHERAPY SCHEDULING PROBLEM - DE CALENDARIZACION DE TRATAMIENTOS DE RADIOTERAPIA
//UTILIZANDO SIMULATED ANNEALING
//
//
//LA REPRESENTACION DONDE SE GUARDA UNA SOLUCION CORRESPONDE A DOS MATRICES
//CADA UNA ES UN VECTOR DE VECTORES - repDoc y repMaq -
//CADA MATRIZ SE INICIALIZA EN MAIN CON CEROS
//Cada matriz representa el calendario de 20 dias habiles (no se considera fines de semana)
//cada dia con 16 bloques de media hora.
//existe una columna por cada bloque y una fila por cada paciente.
//en total 20 dias habiles: 320 columnas
//cantidad de filas (pacientes) depende de la instancia

//los pacientes son de 3 tipos y requieren distintas cantidades de sesiones a agendar
//urgentes - 2 sesiones
//paliativos - 4 sesiones
//radicales - todas las sesiones que alcancen desde el dia aleatorio que les toco 
//(radicales en teoria 30 sesiones pero no alcanzaran en el calendario)


//- FUNCION INICIAR -
//GENERA UNA SOLUCION INICIAL ALEATORIA
//
//COMIENZA CON UN PACIENTE ALEATORIO EN UN DIA ALEATORIO DE LA PRIMERA SEMANA
//RECONOCE EL TIPO DE PACIENTE Y AGENDA LAS SESIONES NECESARIAS
//EN LO POSIBLE EN DIAS SEGUIDOS PERO CON LA FLEXIBILIDAD QUE SI NO ENCUENTRA BLOQUES DISPONIBLES
//PUEDE SEGUIR BUSCANDO AL DIA SIGUIENTE, Y ASI SUCESIVAMENTE.
//LA FUNCION NECESITA SABER SI HAY DOCS/MAQUINAS DISPONIBLES EN UN BLOQUE ESPECIFICO
//PARA ELLO CUENTA DOS MATRICES QUE CONTIENEN LOS DOCTORES DISPONIBLES PARA CADA DIA
//LO MISMO PARA MAQUINAS CON SU RESPECTIVA MATRIZ - docs_detalle y maq_detalle - 
//EN LA MEDIDA QUE ENCUENTRA BLOQUE DISPONIBLE, INGRESA LOS DATOS A LOS VECTORES:
//agendaP: cada [i] contiene las sesiones agendadas de un paciente.(el bloque global entre 0 y 319)
//doctoresP: cada [i] contiene los doctores agendados de un paciente (el numero o ID del doctor)
//maquinasP: cada [i] contiene las maquinas ocupadas por un paciente (el numero o ID de maquina)
//EN LA MEDIDA QUE SE VAN AGREGANDO, TAMBIEN SE ELIMINAN DE LA LISTA DE MEDICOS Y MAQUINAS DISPONIBLES
//AQUELLAS QUE HAN SIDO AGENDADAS.




//SOLUCION INICIAL ALEATORIA

void iniciar(int npac, int ur, int pal, int rad, vector<int> &pacientes, vector<vector<int>> &agendaP, vector<vector<int>> &doctoresP, vector<vector<int>> &maquinasP, vector<vector<int>> detalleD, vector<vector<int>> detalleM){
    
    srand(time(0));
    // dia random de la primera semana
    int diarand = rand() % 5 + 1;

    // paciente random apto para cualquiera de las instancias
    int pacrand = rand() % 15;

    //determinar que tipo de paciente es para agendar la cantidad de sesiones correspondientes

    if(pacrand < ur){
        //paciente urgente
        int ok = 0;
        int aux = 0;        

        while(ok < 2){ // 2 sesiones
            if((aux + diarand)>19){ //verifica que no se salga del calendario
                break;
            }
            int bloques = 0;
            
            while (bloques<16){
                int bglobal2 = (16*(diarand -1 +aux)) + bloques; //ubicacion global del bloque
                
                if ((detalleM[bglobal2].size() >= 1) && (detalleD[bglobal2].size() >= 1)){  // verifica si hay disponibilidad tanto de maquinas como doctores en ese bloque
                
                    agendaP[pacrand].push_back(bglobal2);                      //agrega el bloque a la lista de sesiones del paciente actual
                    maquinasP[pacrand].push_back(detalleM[bglobal2][0]);       //agrega la primera maquina disponible a la lista de maquinas del paciente actual
                    doctoresP[pacrand].push_back(detalleD[bglobal2][0]);       //agrega el primer doctor disponible a la lista de doctores del paciente actual
                    detalleM[bglobal2].erase(detalleM[bglobal2].begin() + 0);  //elimina la maquina de la lista de disponibles
                    detalleD[bglobal2].erase(detalleD[bglobal2].begin() + 0);  //elimina al doctor de la lista de disponibles
                    ok++;
                    bloques = 17;

                } else {
                    bloques++;
                }
            }
            aux++;
        }
        pacientes[pacrand]=1; //registra tipo de paciente en vector pacientes
        
    } else if (pacrand < (ur+pal)){ //paciente paliativo
        int ok = 0;
        int aux = 0;

        while(ok < 4){//hasta agendar las 4 sesiones
            if ((aux + diarand)>19){
                break;
            }
            int bloques = 0;
            
            while (bloques<16){
                int bglobal2 = (16*(diarand-1+aux)) + bloques; //ubicacion global del bloque 
                
                if ((detalleM[bglobal2].size() >= 1) && (detalleD[bglobal2].size() >= 1)){
                
                    agendaP[pacrand].push_back(bglobal2);
                    maquinasP[pacrand].push_back(detalleM[bglobal2][0]);
                    doctoresP[pacrand].push_back(detalleD[bglobal2][0]);
                    detalleM[bglobal2].erase(detalleM[bglobal2].begin() + 0);
                    detalleD[bglobal2].erase(detalleD[bglobal2].begin() + 0);
                    ok++;
                    bloques = 17;

                } else {
                    bloques++;
                }
            }
            aux++;
        }
        pacientes[pacrand]=2; //registra tipo de paciente en vector pacientes

    } else{
    //paciente radical
    
        int ok = 0;
        int aux = 0;
        int dias = 20 - diarand; //cuantos dias se pueden agendar para ese p radical

        while(ok < dias){// agendar todas las sesiones q alcancen de las 30 en el actual calendario desde el dia random
            if ((aux + diarand) > 19){
                break;
            }
            int bloques = 0;
            
            while (bloques < 16){
                int bglobal2 = (16*(diarand-1 +aux)) + bloques; //ubicacion global del bloque

                if ((detalleM[bglobal2].size() >= 1) && (detalleD[bglobal2].size() >= 1)){

                    agendaP[pacrand].push_back(bglobal2);
                    maquinasP[pacrand].push_back(detalleM[bglobal2][0]);
                    doctoresP[pacrand].push_back(detalleD[bglobal2][0]);
                    detalleM[bglobal2].erase(detalleM[bglobal2].begin() + 0); //borra la maquina de la lista disponible
                    detalleD[bglobal2].erase(detalleD[bglobal2].begin() + 0); //borra el doctor de la lista disponible
                    ok++;
                    bloques = 17;

                } else {
                    bloques++;
                    
                }
            }
            aux++;
        }
        pacientes[pacrand]=3; //registra tipo de paciente en vector pacientes        

    }   


    //agendar el resto de los pacientes
        
    //pacientes urgentes
    for (int a = 0; a < ur ;a++){
        if(a == pacrand){ //se salta al paciente inicial que ya se agendo
            a+=1;
        }
        int ok = 0;
        int aux = 0;
        int diarand2 = rand() % 19 + 1; // un dia random 

        while(ok<2){//hasta agendar las 2 sesiones
            if((aux+diarand2)>19){
                break;
            }
            int bloques1 = 0;
            
            while (bloques1<16){
                int bglobal2 = (16*(diarand2 -1 +aux)) + bloques1; //ubicacion global del bloque random dia x

                if ((detalleM[bglobal2].size() >= 1) && (detalleD[bglobal2].size() >= 1)){
                
                    agendaP[a].push_back(bglobal2);
                    maquinasP[a].push_back(detalleM[bglobal2][0]);
                    doctoresP[a].push_back(detalleD[bglobal2][0]);
                    detalleM[bglobal2].erase(detalleM[bglobal2].begin() + 0);
                    detalleD[bglobal2].erase(detalleD[bglobal2].begin() + 0);
                    ok++;
                    bloques1 = 17;

                } else {
                    bloques1++;
                }
            }
            aux++;
        }
        pacientes[a]=1; //registra tipo de paciente en vector pacientes  
    }
    //pacientes paliativos
    for (int b = ur; b < (ur+pal) ;b++){
        if(b == pacrand){ //se salta al paciente inicial que ya se agendo
            b+=1;
        }
        
        int ok = 0;
        int aux = 0;
        int diarand2 = rand() % 15 + 3; // un dia random entre el 1 y el 17

        while(ok<4){//hasta agendar las 4 sesiones
            if((aux+diarand2)>19){
                break;
            }
            int bloques2 = 0;
            
            while (bloques2<16){
                int bglobal2 = (16*(diarand2 -1 + aux)) + bloques2; //ubicacion global del bloque random dia x

                if ((detalleM[bglobal2].size() >= 1) && (detalleD[bglobal2].size() >= 1)){
                
                    agendaP[b].push_back(bglobal2);
                    maquinasP[b].push_back(detalleM[bglobal2][0]);
                    doctoresP[b].push_back(detalleD[bglobal2][0]);
                    detalleM[bglobal2].erase(detalleM[bglobal2].begin() + 0);
                    detalleD[bglobal2].erase(detalleD[bglobal2].begin() + 0);
                    ok++;
                    bloques2 = 17;

                } else {
                    bloques2++;
                }
            }
            aux++;
        }
        pacientes[b] = 2; //registra tipo de paciente en vector pacientes

    } 
    //pacientes radicales
    for (int c = (pal+ur); c < npac ;c++){
        if(c == pacrand){ //se salta al paciente inicial que ya se agendo
            c+=1;
        }
        
        int ok = 0;
        int aux = 0;
        int diarand2 = rand() % 20 + 15; // un dia random entre el 15 y el 20
        int dias = 20 - diarand2 + 1; //cuantos dias quedan del calendario

        while(ok<dias){//hasta agendar lo que se alcance de las 30 sesiones
            if((aux+diarand2)>19){
                break;
            }
            int bloques3 = 0;
            
            while (bloques3<16){
                int bglobal2 = (16*(diarand2 -1 +aux)) + bloques3; //ubicacion global del bloque random dia x

                if ((detalleM[bglobal2].size() >= 1) && (detalleD[bglobal2].size() >= 1)){
                
                    agendaP[c].push_back(bglobal2);
                    maquinasP[c].push_back(detalleM[bglobal2][0]);
                    doctoresP[c].push_back(detalleD[bglobal2][0]);
                    detalleM[bglobal2].erase(detalleM[bglobal2].begin() + 0);
                    detalleD[bglobal2].erase(detalleD[bglobal2].begin() + 0);
                    ok++;
                    bloques3 = 17;

                } else {
                    bloques3++;
                }
            }
            aux++;
        }
        pacientes[c]=3; //registra tipo de paciente en vector pacientes

    }

    return;
}


// funcion que genera nueva solucion candidata
vector<vector<vector<int>>> generar_vecino(int npac, int ur, int pal, int rad, vector<int> listaP, vector<vector<int>> d_detalle, vector<vector<int>> m_detalle){
    //devuelve un vector triple donde
    //[0] para agendap
    //[1] para docs 
    //[2] para maqs
    //[3] para pacientes
    
    //se crean e inicializan los vectores
    vector<vector<vector<int>>> vecinx;
    vecinx.assign(4,vector<vector<int>>(npac,vector<int>()));
    
    vector<vector<int>> agendaP;
    vector<vector<int>> doctoresP;
    vector<vector<int>> maquinasP;
    vector<vector<int>> pacientes2;
    agendaP.assign(npac, vector<int>());
    doctoresP.assign(npac, vector<int>());
    maquinasP.assign(npac, vector<int>());
    pacientes2.assign(npac, vector<int>());


    // randomizar lista de pacientes y agendar en ese orden
    random_shuffle(listaP.begin(), listaP.end());

    for(int x = 0; x < listaP.size(); x ++){
        if(listaP[x] < ur){ //paciente urgente
            pacientes2[x].push_back(1); //ID identifica tipo de paciente en vector pacientes
            int ok = 0;
            int aux = 0;
            int diarand3 = rand() % 3 + 2; //restriccion

            while(ok<2){  //2 sesiones
                if((aux+diarand3)>19){
                    break;
                }
                int bloques = 0;
                
                while (bloques<16){ //maneja los bloques de un dia
                    int bglobal2 = (16*(diarand3 -1 +aux)) + bloques; //ubicacion global del bloque

                    if ((m_detalle[bglobal2].size() >= 1) && (d_detalle[bglobal2].size() >= 1)){
                    

                        agendaP[x].push_back(bglobal2);
                        maquinasP[x].push_back(m_detalle[bglobal2][0]);
                        doctoresP[x].push_back(d_detalle[bglobal2][0]);
                        m_detalle[bglobal2].erase(m_detalle[bglobal2].begin() + 0);
                        d_detalle[bglobal2].erase(d_detalle[bglobal2].begin() + 0);
                        
                        ok++;
                        bloques = 17;

                    } else {
                        bloques++;
                    }
                }
                aux++;
            }
        } else if(listaP[x] < (ur+pal)){ //paciente paliativo
            pacientes2[x].push_back(2); //ID identifica paciente paliativo en vector pacientes
            int ok = 0;
            int aux = 0;
            int diarand4 = rand() % 15 + 3; //restriccion

            while(ok < 4){  //4 sesiones
                if((aux+diarand4)>19){
                    break;
                }
                int bloques1 = 0;
                
                while (bloques1<16){
                    int bglobal2 = (16*(diarand4 -1 +aux)) + bloques1; //ubicacion global del bloque

                    if ((m_detalle[bglobal2].size() >= 1) && (d_detalle[bglobal2].size() >= 1)){

                        agendaP[x].push_back(bglobal2);
                        maquinasP[x].push_back(m_detalle[bglobal2][0]);
                        doctoresP[x].push_back(d_detalle[bglobal2][0]);
                        m_detalle[bglobal2].erase(m_detalle[bglobal2].begin() + 0);
                        d_detalle[bglobal2].erase(d_detalle[bglobal2].begin() + 0);
                        
                        ok++;
                        bloques1 = 17;

                    } else {
                        bloques1++;
                    }
                }
                aux++;
            }
        } else { //pac radical
            pacientes2[x].push_back(3); //ID identifica paciente urgente en vector pacientes
            int ok = 0;
            int aux = 0;
            int diarand5 = rand() % 20 + 15;
            int diasr = 20 - diarand5 + 1; //cuantos dias quedan del calendario

            while(ok < diasr){
                if((aux+diarand5)>19){ // sale del loop al superar [19] que es el dia 20, ultimo del calendario
                    break;
                }
                int bloques2 = 0;
                
                while (bloques2<16){
                    int bglobal2 = (16*(diarand5 -1 +aux)) + bloques2; //ubicacion global del bloque random dia x

                    if ((m_detalle[bglobal2].size() >= 1) && (d_detalle[bglobal2].size() >= 1)){

                        agendaP[x].push_back(bglobal2);
                        maquinasP[x].push_back(m_detalle[bglobal2][0]);
                        doctoresP[x].push_back(d_detalle[bglobal2][0]);
                        m_detalle[bglobal2].erase(m_detalle[bglobal2].begin() + 0);
                        d_detalle[bglobal2].erase(d_detalle[bglobal2].begin() + 0);
                        
                        ok++;
                        bloques2 = 17;

                    } else {
                        bloques2++;
                    }
                }
                aux++;
            }
        }
    }

    vecinx[0]=agendaP;
    vecinx[1]=doctoresP;
    vecinx[2]=maquinasP;
    vecinx[3]=pacientes2;

    return vecinx;
}



//EVALUACION

//Funcion que mide el tiempo de espera total de todos los pacientes antes de su primera sesion
//**Solo considera a los que lograron tener al menos una sesion agendada**

//agendaP contiene para cada paciente los bloques de tratamiento en orden
//esta funcion toma el primer elemento de cada paciente que representa un bloque global
//en el calendario, simplemente se divide por 16 para calcular los dias que esperó
//antes de iniciar el tratamiento. Se repite para cada paciente y se obtiene el tiempo total
//******PENALIZACION SOLUCIONES NO FACTIBLES******
//por cada paciente que espera fuera del rango establecido, suma 10,000 como penalizacion

vector<double> tiempoEsperaTotal(int ur, int pal, int rad, vector<int> pacientes, vector<vector<int>> agendaP){
    vector<double> info; //devuelve el tiempo total y la cantidad de pacientes no factibles
    info.assign(2,0);
    double tiempoTotal = 0;
    int infac = 0;

    for (int i=0 ; i<(ur+pal+rad) ; i++){

        if(agendaP[i].size()>=1){
            if(pacientes[i] == 1){ //paciente urgente puede esperar entre 1 y 2 dias
                
                if((agendaP[i][0]>47) || (agendaP[i][0]<16)){ //fuera de la restriccion
                    tiempoTotal += 10000; //penalizacion de sol no factible
                    //tiempoTotal+= (agendaP[i][0]/16);
                    infac+=1;

                }else {
                    tiempoTotal+= (agendaP[i][0]/16);

                }
            } else if(pacientes[i] == 2){ //paciente  palitivo espera entre 2 y 14 dias
                
                if((agendaP[i][0]>((14*16)+15)) || (agendaP[i][0]<32)){ //fuera de la restriccion
                    tiempoTotal += 10000; //penalizacion de sol no factible
                    //tiempoTotal+= (agendaP[i][0]/16);
                    infac+=1;

                }else {
                    tiempoTotal+= (agendaP[i][0]/16);

                }
            } else if(pacientes[i] == 3){ //paciente radical espera entre 14 y 28 dias
                
                if(agendaP[i][0]<(16*14)){ //fuera de la restriccion
                    tiempoTotal += 10000; //penalizacion de sol no factible
                    //tiempoTotal+= (agendaP[i][0]/16);
                    infac+=1;

                }else {
                    tiempoTotal+= (agendaP[i][0]/16);
                
                }
            }
        }
    }

    info[0]=tiempoTotal;
    info[1]=infac;
    return info;
}


//SIMULATED ANNEALING 

bool comparacion(double &actual, double nueva, double temp){
    double criterio;
    if (nueva < actual){
        criterio = 1;
    }
    else{
        criterio = exp((actual-nueva)/temp);
    }
    double c = (double) rand()/RAND_MAX;

    if (criterio > c){
        actual = nueva;
        return true;

    } else {
        return false;
    }
}

void algoritmoSA(int npac, int ur, int pal, int rad, vector<int> &pacientes, vector<int> listaP, vector<vector<int>> &agendaP, vector<vector<int>> &doctoresP, vector<vector<int>> &maquinasP){
    
    double T = 100;
    vector<double> actual = tiempoEsperaTotal(ur, pal, rad, pacientes, agendaP); //tiempo total de solucion inicial
    double mejor = 10000000;
    int salida;
    int iterTotal = 0;
    int infac = 0;
    // CONTROL DE ITERACIONES 
    while(iterTotal < 20000 || mejor > 10000){
        cout << "Procesando...     ";

        //SALIDA DEL ALGORITMO AL ALCANZAR ESTA TEMPERATURA
        if (T <= 0.0001){
            break;
        }
        //if (iterTotal == 10000){
        //    break;
        //}
        int it_sinmejora = 0; //mide cantidad de iteraciones sin mejorar calidad de soluciones
        for (int i = 0 ; i<20 ; i++){ //iteraciones en una misma temperatura

            //generar vecino contempla un vector agendaP de triple largo i ya que contiene los docs y maqs candidatos tambien
            vector<vector<vector<int>>> vecino = generar_vecino(npac, ur, pal, rad, listaP, docs_detalle, maq_detalle); //genera nueva solucion candidata
            vector<vector<int>> candidata;  // agendaP candidata
            vector<vector<int>> candidata_docs; // doctoresP candidato
            vector<vector<int>> candidata_maqs; //maquinasP candidata
            vector<int> pacs;
            pacs.assign(npac,0);
            candidata = vecino[0];
            candidata_docs = vecino[1];
            candidata_maqs = vecino[2];
            
            for(int a = 0 ; a < npac ; a++){
                if(vecino[3][a].size()>=1){
                    int x =vecino[3][a][0];
                    pacs[a]=x;
                }
            }
                
            vector<double> nueva = tiempoEsperaTotal(ur, pal, rad, pacs, candidata); //obtiene tiempo total de espera de la solucion candidata
            if (comparacion(actual[0], nueva[0], T) == true){  //evalua la calidad de las soluciones actual y nueva - Si es true entonces nueva es mejor que actual
                
                //compara con la mejor hasta el momento
                if (nueva[0] < mejor){
                    mejor = nueva[0];
                    infac = int(nueva[1]);
                    agendaP = candidata; //modifica la agenda final por la mejor hasta el momento
                    doctoresP = candidata_docs; //modifica agenda doctores
                    maquinasP = candidata_maqs; //modificaa agenda maquinas
                    pacientes = pacs;    //ajusta tipo de pacientes a los correspondientes

                    if(infac>0){
                        salida = mejor - (infac*10000); // si hay pacientes infactibles al final de todas las iteraciones, entrega el mejor tiempo sin la penalizacion (pero avisa que la solucion es infactible)
                    }
                    cout << "Se ha encontrado una mejor solucion! \n";
                    
                }
            } else{
                it_sinmejora ++;
            }
            iterTotal ++ ;
        }
        //RECALENTAMIENTO
        if (it_sinmejora > 17){ //si en una misma temperatura no mejora en 18/20 iteraciones
            T+= 0.03;
        
        }
        //ENFRIAMIENTO
        if((T-0.1) >= 0.0001){
            T -= 0.1;
        }else{
            T = 0.0001;
        }
        
    }

    
    cout << "\n\n\nSimulated annealing aplicado, guardando solucion... \n";
    cout << "\nTemperatura inicial: 100 \n";
    cout << "Temperatura final: " << T << "\n";
    cout << "Iteraciones realizadas: " << iterTotal << "\n";
    cout << "\nEl mejor tiempo total de espera encontrado es: " << salida << " dias \n";
    cout << "Pacientes agendados que no cumplieron restriccion de dias de espera: " << infac << "\n";
    


    return;      
}
     



//ESCIBIR EN REPRESENTACION FINAL
//
void transcribirSolFinal(vector<vector<int>> agendaP, vector<vector<int>> doctoresP, vector<vector<int>> maquinasP, vector<vector<int>> &repDoc, vector<vector<int>> &repMaq){
    for (int i=0 ; i<agendaP.size() ; i++){
        for (int j=0 ; j<agendaP[i].size() ; j++){
            if(agendaP[i][j] >= 1){
                int aux = agendaP[i][j];  //aux es el bloque global
                repDoc[i][aux] = doctoresP[i][j]; //agenda el doctor de la sesion en bloque global del paciente i
                repMaq[i][aux] = maquinasP[i][j]; //agenda la maquina en bloque global del paciente i
            }
        }
    }
    //cout << "Se ha transcrito la solucion a la representacion final, \n" ;
    return;
}
     


//ESCRIBIR ARCHIVOS SALIDA
//escribir solucion final
    
void escribir_resultados(vector<vector<int>> docs, vector<vector<int>> maqs){
    ofstream tabla_d;  
    tabla_d.open ("resultadosA.txt");
    for (int i = 0; i<docs.size() ;i++){
        for (int j = 0; j<docs[i].size() ;j++){
            int elem = docs[i][j];
            tabla_d << to_string(elem) + " "; //ingresa los datos de una fila
        }
        tabla_d << "\n";
    }
    tabla_d.close();

        
    ofstream tabla_m;  
    tabla_m.open ("resultadosB.txt");
    for (int k = 0; k<maqs.size() ;k++){
        for (int l = 0; l<maqs[k].size() ;l++){
            int elemm = maqs[k][l];
            tabla_m << to_string(elemm) + " "; //ingresa los datos de una fila
        }
        tabla_m << "\n";
    }
    tabla_m.close();
  
    //cout << "\nResultados escritos con éxito en archivos txt \n";
    return;
}



//MAIN
int main(){
    int nmaq;
    int ndoc;
    int npac;
    int pacr;
    int pacp;
    int pacu;
    int instancia;


    cout << " Ingrese número de instancia: ";
    cin >> instancia;

    if (instancia == 1){


        nmaq = 2;
        ndoc = 3;
        npac = 15;
        pacr = 2;
        pacp = 3;
        pacu = 10;

        //vector que ayuda a identificar tipo de paciente
        //1: urgente - 2: paliativo - 3:radical
        //inicialmente todos en 0 - no clasificados
        pacientes.assign(npac, 0);
        
        for(int p = 0; p < npac; p++){ 
            listaP.push_back(p);          //vector con los pacientes
        }
        
        //se asigna a la matriz la cantidad de filas con cantidad de pacientes
        //la cantidad de columnas es constante
        //se inicializa con 0 en todos los bloques
        repDoc.assign(15, vector<int>(320,0));
        repMaq.assign(15, vector<int>(320,0));
        
        agendaP.assign(15, vector<int>());
        doctoresP.assign(15, vector<int>());
        maquinasP.assign(15, vector<int>());

        //turnos medicos de la instancia 1
        turnos = { { 1, 0, 2, 1, 0 }, 
                             { 0,2,1,0,2 }, 
                             { 1,1,1,0,1 } }; 
        
        //que doctores se encuentran segun horario del dia, mañana o tarde, de lunes a viernes
        // por ej el index 0 es lunes en la mañana y estaran los doctores 1 y 3
        horarios_docs = {{1,3},{},{3},{2},{2,3},{1},{1},{},{3},{2}};
        
        
        // llena vector de doctores disponibles para cada bloque de las 4 semanas: 320 vectores internos
        
        for (int a=0 ; a < 4 ; a++){
            for (int b=0 ; b < 10 ; b++){
                for (int c=0 ; c < 8 ; c++){
                    docs_detalle.push_back(horarios_docs[b]);
                }
            }
        }
        
        // llena vector de maquinas disponibles para cada bloque de las 4 semanas: 320 vectores internos
        maquinas = {1,2};  // 2 maquinas disponibles en cada bloque  
        for (int d=0 ; d < 320 ; d++){
            maq_detalle.push_back(maquinas);
        }


    } else if (instancia == 3){


        nmaq = 5;
        ndoc = 7;
        npac = 50;
        pacr = 5;
        pacp = 5;
        pacu = 40;

        //vector que ayuda a identificar tipo de paciente
        //1: urgente - 2: paliativo - 3:radical
        //inicialmente todos en 0 - no clasificados
        pacientes.assign(npac, 0);

        for(int e = 0; e < npac; e++){ 
            listaP.push_back(e);          //vector con los pacientes
        }
        
        //se asigna a la matriz la cantidad de filas con cantidad de pacientes
        //la cantidad de columnas es constante
        //se inicializa con 0 en todos los bloques
        repDoc.assign(50, vector<int>(320,0));
        repMaq.assign(50, vector<int>(320,0));

        agendaP.assign(50, vector<int>());
        doctoresP.assign(50, vector<int>());
        maquinasP.assign(50, vector<int>());

        //turnos medicos de la instancia 3        
        turnos = { { 0,1,2,0,1 }, 
                             { 1,0,0,3,0 }, 
                             { 1,0,1,1,2 },
                             { 1,3,1,0,3 },
                             { 0,1,2,2,0 },
                             { 2,0,1,1,1 },
                             { 2,2,0,0,2 } }; 
        
        //que doctores se encuentran segun horario del dia, mañana o tarde, de lunes a viernes
        horarios_docs = { {2,3,4},{6,7},{1,4,5},
                                          {4,7},
                                          {3,4,6},
                                          {1,5},
                                          {2,3,6},
                                          {2,5},
                                          {1,4,6},
                                          {3,4,7}};
        
           
        // llena vector de doctores disponibles para cada bloque de las 4 semanas: 320 vectores internos
        for (int f=0 ; f < 4 ; f++){
            for (int g=0 ; g < 10 ; g++){
                for (int h=0 ; h < 8 ; h++){
                    docs_detalle.push_back(horarios_docs[g]);
                }
            }
        }

        // crear vector de maquinas disponibles para cada bloque de las 4 semanas: 320 vectores internos
        maquinas = {1,2,3,4,5};  // 5 maquinas disponibles en cada bloque
        for (int i=0 ; i < 320 ; i++){
            maq_detalle.push_back(maquinas);
        }


    } else if (instancia == 4){


        nmaq = 8;
        ndoc = 10;
        npac = 65;
        pacr = 10;
        pacp = 7;
        pacu = 48;
       

        //vector que ayuda a identificar tipo de paciente
        //1: urgente - 2: paliativo - 3:radical
        //inicialmente todos en 0 - no clasificados
        pacientes.assign(npac, 0);

        for(int q = 0; q < npac; q++){ 
            listaP.push_back(q);          //vector con los pacientes
        }
        
        //se asigna a la matriz la cantidad de filas con cantidad de pacientes
        //la cantidad de columnas es constante
        //se inicializa con 0 en todos los bloques
        repDoc.assign(65, vector<int>(320,0));
        repMaq.assign(65, vector<int>(320,0));

        agendaP.assign(65, vector<int>());
        doctoresP.assign(65, vector<int>());
        maquinasP.assign(65, vector<int>());
        
        //turnos medicos de la instancia 4
        turnos = { { 1,0,1,0,2 }, 
                             { 2,1,0,0,1 }, 
                             { 1,0,2,3,0 },
                             { 0,1,3,1,0 },
                             { 1,3,3,0,0 },
                             { 0,2,1,0,1 },
                             { 2,0,0,3,2 },
                             { 0,0,1,3,3 },
                             { 2,0,2,0,0 },
                             { 1,1,0,2,0 } };
        
        //que doctores se encuentran segun horario del dia, mañana o tarde, de lunes a viernes
        //por ejemplo lunes en la mañana estan los doctores 1,3,5,10
        horarios_docs = {{1,3,5,10},{2,7,9},{2,4,5,10},
                                           {5,6},
                                           {1,4,5,6,8},
                                           {3,4,5,9},
                                           {3,4,7,8},
                                           {3,7,8,10},
                                           {2,6,8},
                                           {1,7,8}};    
        
        
        // llena vector de doctores disponibles para cada bloque de las 4 semanas: 320 vectores internos
        for (int j=0 ; j < 4 ; j++){
            for (int k=0 ; k < 10 ; k++){
                for (int l=0 ; l < 8 ; l++){
                    docs_detalle.push_back(horarios_docs[k]);
                }
            }
        }

        // crear vector de maquinas disponibles para cada bloque de las 4 semanas: 320 vectores internos
        maquinas = {1,2,3,4,5,6,7,8};  // 8 maquinas disponibles en cada bloque
        for (int m=0 ; m < 320 ; m++){
            maq_detalle.push_back(maquinas);
        }


    } else {
        cout << "\n---Número Inválido---\n";
        return 0;
    }



    //solucion inicial aleatoria
    iniciar(npac, pacu, pacp, pacr, pacientes, agendaP, doctoresP, maquinasP, docs_detalle, maq_detalle);
    
    //aplicar algoritmo SA
    algoritmoSA(npac, pacu, pacp, pacr, pacientes, listaP, agendaP, doctoresP, maquinasP);

    //pasar de agenda a representacion final
    transcribirSolFinal(agendaP, doctoresP, maquinasP, repDoc, repMaq);

    //exportar el resultado a formato de salida
    escribir_resultados(repDoc, repMaq);
    
    int sinhora_u = 0;
    int sinhora_p = 0;
    int sinhora_r = 0;
    for(int n = 0 ; n < npac ; n++){
        if(agendaP[n].size() < 1){
            if(pacientes[n]==3){
                sinhora_r += 1;
            } else if(pacientes[n]==2){
                sinhora_p += 1;
            } else if(pacientes[n]==1){
                sinhora_u += 1;
            }
        }
    }

    cout << "\nPacientes que quedaron sin ninguna sesion agendada: \n";
    cout << "Urgentes: " << sinhora_u << "\nPaliativos: " << sinhora_p << "\nRadicales: " << sinhora_r << "\n\n"; 
    cout << "--Resultados entregados corresponden a instancia: " << instancia << "--\n";

    return 0;
}