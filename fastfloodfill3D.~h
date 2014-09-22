
typedef struct rango Rango;

struct rango
{
bool split;
bool isempty;

int left[2];   //Can be composed by 2 subregions
int right[2];

};

struct region
{
int y;
int z;
rango *rN,*rS,*rU,*rD;
};

//-----------------------------------------------------------------------------

void fastfloodfill3D( Volumen* M,
                      short int upThr, short int lowThr,
                      short int fillClr,
                      short int x0, short int y0, short int z0)
{
//x0,y0: seed point for flood fill with color floodClr every connected pixel in image I
//       that is in the range [lowThr, upThr]


    //First region detection:
    TList* Regiones = new TList;

    int x=x0;
    int y=y0;
    int z;

    int left, right;

    int N=1;

    while ( M->GetV(x,y,z0)<upThr && M->GetV(x,y,z0)>lowThr )
    {
        M->SetV(x,y,z0,fillClr);
        x--;
    }

    left = x+1;

    x=x0+1;

    while ( M->GetV(x,y,z0)<upThr && M->GetV(x,y,z0)>lowThr )
    {
        M->SetV(x,y,z0,fillClr);
        x++;
    }

    right = x-1;

    region* nR = new region;

    rango *rN, *rS, *rU, *rD;

    rN = new rango;
    rS = new rango;
    rU = new rango;
    rD = new rango;

    rN->split    = false;
    rN->isempty  = false;
    rS->split    = false;
    rS->isempty  = false;
    rU->split    = false;
    rU->isempty  = false;
    rD->split    = false;
    rD->isempty  = false;

    rN->left[0]  = left;
    rS->left[0]  = left;
    rU->left[0]  = left;
    rD->left[0]  = left;

    rN->right[0] = right;
    rS->right[0] = right;
    rU->right[0] = right;
    rD->right[0] = right;

    nR->y  = y0;
    nR->z  = z0;
    nR->rN = rN;
    nR->rS = rS;
    nR->rU = rU;
    nR->rD = rD;

    Regiones->Add(nR);

    //Flood Fill Auxiliary pointers:
    region* pR;  //actual region
    region* pNR; //new region

    rango *nrN, *nrS, *nrU, *nrD;

    int xi;
    int xf;

    int r;

    while(Regiones->Count)
    {
        pR = (region*)Regiones->First();

        //Búsqueda North Regions:
        rN = pR->rN;

        if(!rN->isempty)
        {
            N=1;
            if(rN->split)
            N=2;

           for(r=0; r<N; r++)
           {
            x0 = rN->left[r];
            xf = rN->right[r];

            y = pR->y+1;  //North direction
            z = pR->z;

            for(x=x0; x<=xf; x++ )
            {
                if(M->GetV(x,y,z)>=upThr || M->GetV(x,y,z)<=lowThr)
	     	  continue;

                xi = x;

                while( M->GetV(x,y,z)<upThr && M->GetV(x,y,z)>lowThr )
                {
                    M->SetV(x,y,z,fillClr);
                    x--;
                }

                left = x+1;

                x = xi+1;

                while( M->GetV(x,y,z)<upThr && M->GetV(x,y,z)>lowThr )
                {
                    M->SetV(x,y,z,fillClr);
                    x++;
                }

                right = x-1;

                x = right+1;

                if(left!=right)//New region found, connected to this point;
                {
                    if(left<x0-1 && right<=xf)
                    {
                        //  left ----- right
                        //        x0--------xf
                       nrS = new rango;
                       nrS->split=false;
                       nrS->isempty=false;
                       nrS->left[0]=left;
                       nrS->right[0]=x0-2;
                    }

                    if(left>=x0-1 && right<=xf+1)
                    {
                        //  left ---- right
                        //   x0--------xf
                        nrS = new rango;
                        nrS->isempty=true;
                    }

                    if(left>=x0-1 && right>xf+1)
                    {
                        //      left ---- right
                        //   x0--------xf
                        nrS = new rango;
                        nrS->split=false;
                        nrS->isempty=false;
                        nrS->left[0]=xf+2;
                        nrS->right[0]=right;
                    }

                    if(left<x0-1 && right>xf+1)
                    {
                        //  left------------ right
                        //      x0--------xf
                        nrS = new rango;
                        nrS->split=true;
                        nrS->isempty=false;
                        nrS->left[0]=left;
                        nrS->right[0]=x0-2;
                        nrS->left[1]=xf+2;
                        nrS->right[1]=right;
                    }

                    pNR = new region;

                    nrN = new rango;
                    nrN->split=false;
                    nrN->isempty=false;
                    nrN->left[0]=left;
                    nrN->right[0]=right;

                    pNR->rN = nrN;

                    pNR->rS = nrS;

                    nrU = new rango;
                    nrU->split=false;
                    nrU->isempty=false;
                    nrU->left[0]=left;
                    nrU->right[0]=right;

                    pNR->rU = nrU;

                    nrD = new rango;
                    nrD->split=false;
                    nrD->left[0]=left;
                    nrD->right[0]=right;

                    pNR->rD = nrD;

                    pNR->y = y;
                    pNR->z = z;

                    Regiones->Add(pNR);
                }// if(left!=right)
            }//end of for(int x=x0; x<=xf; x++ )
          }//end for(int r=0;r<N;r)
        } //end if(!rN->isempty)

        //Búsqueda South Regions:
        rS = pR->rS;

        if(!rS->isempty)
        {
            N=1;
            if(rS->split)
            N=2;

            for(r=0; r<N;r++)
            {
            x0 = rS->left[r];
            xf = rS->right[r];

            y = pR->y-1;  //South direction
            z = pR->z;

            for(x=x0; x<=xf; x++ )
            {
                if(M->GetV(x,y,z)>=upThr || M->GetV(x,y,z)<=lowThr)
		continue;

                xi = x;

                while( M->GetV(x,y,z)<upThr && M->GetV(x,y,z)>lowThr )
                {
                    M->SetV(x,y,z,fillClr);
                    x--;
                }
                left = x+1;

                x = xi+1;

                while( M->GetV(x,y,z)<upThr && M->GetV(x,y,z)>lowThr )
                {
                    M->SetV(x,y,z,fillClr);
                    x++;
                }
                right = x-1;

                x = right+1;

                if(left!=right)//New region found, connected to this point;
                {
                    if(left>=x0 && right>xf+1)
                    {
                        //  x0-------- xf
                        //      left--------right
                        nrN = new rango;
                        nrN->split=false;
                        nrN->isempty=false;
                        nrN->left[0]=xf+2;
                        nrN->right[0]=right;
                    }

                    if(left>=x0-1 && right<=xf+1)
                    {
                        //    x0---------- xf
                        //     left----right
                        nrN = new rango;
                        nrN->isempty=true;
                    }

                    if(left<x0-1 && right<=xf)
                    {
                        //      x0------- xf
                        // left--------right
                        nrN = new rango;
                        nrN->split=false;
                        nrN->isempty=false;
                        nrN->left[0]=left;
                        nrN->right[0]=x0-2;
                    }

                    if(left<x0-1 && right>xf+1)
                    {
                        //      x0------- xf
                        //left---------------right
                        nrN = new rango;
                        nrN->split=true;
                        nrN->isempty=false;
                        nrN->left[0]=left;
                        nrN->right[0]=x0-2;
                        nrN->left[1]=xf+2;
                        nrN->right[1]=right;
                    }

                    pNR = new region;

                    pNR->rN = nrN;

                    nrS = new rango;
                    nrS->split=false;
                    nrS->isempty=false;
                    nrS->left[0]=left;
                    nrS->right[0]=right;

                    pNR->rS = nrS;

                    nrU = new rango;
                    nrU->split=false;
                    nrU->isempty=false;
                    nrU->left[0]=left;
                    nrU->right[0]=right;

                    pNR->rU = nrU;

                    nrD = new rango;
                    nrD->split=false;
                    nrD->left[0]=left;
                    nrD->right[0]=right;

                    pNR->rD = nrD;

                    pNR->y = y;
                    pNR->z = z;
                    Regiones->Add(pNR);
                }// if(left!=right)
            }//end of for(int x=x0; x<=xf; x++ )
         }//end for(r=0;r<N;r++)
        }//end if(!rS->isempty)

        //Búsqueda Up Regions:
        rU = pR->rU;

        if(!rU->isempty)
        {
            N=1;
            if(rU->split)
            N=2;

            for(r=0; r<N;r++)
            {
            x0 = rU->left[r];
            xf = rU->right[r];

            y= pR->y;
            z= pR->z+1;  //Up direction

            for(x=x0; x<=xf; x++ )
            {
                if(M->GetV(x,y,z)>=upThr || M->GetV(x,y,z)<=lowThr)
		continue;

                xi = x;

                while( M->GetV(x,y,z)<upThr && M->GetV(x,y,z)>lowThr )
                {
                    M->SetV(x,y,z,fillClr);
                    x--;
                }
                left = x+1;

                x = xi+1;

                while( M->GetV(x,y,z)<upThr && M->GetV(x,y,z)>lowThr )
                {
                    M->SetV(x,y,z,fillClr);
                    x++;
                }
                right = x-1;

                x = right+1;

                if(left!=right)//New region found, connected to this point;
                {
                    if(left>=x0 && right>xf+1)
                    {
                        //  x0-------- xf
                        //      left--------right
                        nrD = new rango;
                        nrD->split=false;
                        nrD->isempty=false;
                        nrD->left[0]=xf+2;
                        nrD->right[0]=right;
                    }

                    if(left>=x0-1 && right<=xf+1)
                    {
                        //    x0---------- xf
                        //     left----right
                        nrD = new rango;
                        nrD->isempty=true;
                    }

                    if(left<x0-1 && right<=xf)
                    {
                        //      x0------- xf
                        // left--------right
                        nrD = new rango;
                        nrD->split=false;
                        nrD->isempty=false;
                        nrD->left[0]=left;
                        nrD->right[0]=x0-2;
                    }

                    if(left<x0-1 && right>xf+1)
                    {
                        //      x0------- xf
                        //left---------------right
                        nrD = new rango;
                        nrD->split=true;
                        nrD->isempty=false;
                        nrD->left[0]=left;
                        nrD->right[0]=x0-2;
                        nrD->left[1]=xf+2;
                        nrD->right[1]=right;
                    }

                    pNR = new region;

                    nrN = new rango;
                    nrN->split=false;
                    nrN->isempty=false;
                    nrN->left[0]=left;
                    nrN->right[0]=right;

                    pNR->rN = nrN;

                    nrS = new rango;
                    nrS->split=false;
                    nrS->isempty=false;
                    nrS->left[0]=left;
                    nrS->right[0]=right;

                    pNR->rS = nrS;

                    nrU = new rango;
                    nrU->split=false;
                    nrU->isempty=false;
                    nrU->left[0]=left;
                    nrU->right[0]=right;

                    pNR->rU = nrU;

                    pNR->rD = nrD;

                    pNR->y = y;
                    pNR->z = z;
                    Regiones->Add(pNR);
                }// if(left!=right)
            }//end of for(int x=x0; x<=xf; x++ )
         }//end for(r=0;r<N;r++)
        }//end if(!rS->isempty)

                //Búsqueda Up Regions:
        rD = pR->rD;

        if(!rD->isempty)
        {
            N=1;
            if(rD->split)
            N=2;

            for(r=0; r<N;r++)
            {
            x0 = rD->left[r];
            xf = rD->right[r];

            y= pR->y;
            z= pR->z-1;  //Down direction

            for(x=x0; x<=xf; x++ )
            {
                if(M->GetV(x,y,z)>=upThr || M->GetV(x,y,z)<=lowThr)
		continue;

                xi = x;

                while( M->GetV(x,y,z)<upThr && M->GetV(x,y,z)>lowThr )
                {
                    M->SetV(x,y,z,fillClr);
                    x--;
                }
                left = x+1;

                x = xi+1;

                while( M->GetV(x,y,z)<upThr && M->GetV(x,y,z)>lowThr )
                {
                    M->SetV(x,y,z,fillClr);
                    x++;
                }
                right = x-1;

                x = right+1;

                if(left!=right)//New region found, connected to this point;
                {
                    if(left>=x0 && right>xf+1)
                    {
                        //  x0-------- xf
                        //      left--------right
                        nrU = new rango;
                        nrU->split=false;
                        nrU->isempty=false;
                        nrU->left[0]=xf+2;
                        nrU->right[0]=right;
                    }

                    if(left>=x0-1 && right<=xf+1)
                    {
                        //    x0---------- xf
                        //     left----right
                        nrU = new rango;
                        nrU->isempty=true;
                    }

                    if(left<x0-1 && right<=xf)
                    {
                        //      x0------- xf
                        // left--------right
                        nrU = new rango;
                        nrU->split=false;
                        nrU->isempty=false;
                        nrU->left[0]=left;
                        nrU->right[0]=x0-2;
                    }

                    if(left<x0-1 && right>xf+1)
                    {
                        //      x0------- xf
                        //left---------------right
                        nrU = new rango;
                        nrU->split=true;
                        nrU->isempty=false;
                        nrU->left[0]=left;
                        nrU->right[0]=x0-2;
                        nrU->left[1]=xf+2;
                        nrU->right[1]=right;
                    }

                    pNR = new region;

                    nrN = new rango;
                    nrN->split=false;
                    nrN->isempty=false;
                    nrN->left[0]=left;
                    nrN->right[0]=right;

                    pNR->rN = nrN;

                    nrS = new rango;
                    nrS->split=false;
                    nrS->isempty=false;
                    nrS->left[0]=left;
                    nrS->right[0]=right;

                    pNR->rS = nrS;

                    pNR->rU = nrU;

                    nrD = new rango;
                    nrD->split=false;
                    nrD->isempty=false;
                    nrD->left[0]=left;
                    nrD->right[0]=right;

                    pNR->rD = nrD;

                    pNR->y = y;
                    pNR->z = z;
                    Regiones->Add(pNR);
                }// if(left!=right)
            }//end of for(int x=x0; x<=xf; x++ )
         }//end for(r=0;r<N;r++)
        }//end if(!rS->isempty)

        delete pR;
        Regiones->Delete(0);
    }//end of while(Regiones->Count)

}//end of function
//-----------------------------------------------------------------------------







