/*# -*- coding: utf-8 -*-
Created on Fri Sep 20 19:03:32 2013

@author: khayyam
*/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "bitsCPP.h"
#include "derivatives.h"
#include "tensorFieldUtilsCPP.h"


void integrateTensorFieldProductsCPP(double *q, int *dims, double *diff, double *A, double *b){
    int k=dims[3];
    int nvox=dims[0]*dims[1]*dims[2];
    memset(A, 0, sizeof(double)*k*k);
    memset(b, 0, sizeof(double)*k);
    double *qq=q;
    for(int pos=0;pos<nvox;++pos, qq+=k){
        for(int i=0;i<k;++i){
            b[i]+=qq[i]*diff[pos];
            for(int j=i;j<k;++j){
                A[k*i+j]+=qq[i]*qq[j];
            }
        }
    }
    for(int i=1;i<k;++i){
        for(int j=0;j<i;++j){
            A[k*i+j]=A[k*j+i];
        }
    }
}

void quantizeImageCPP(double *v, int *dims, int numLevels, int *out, double *levels, int *hist){
    if(numLevels<1){
        return;
    }
    memset(hist, 0, sizeof(int)*numLevels);
    memset(levels, 0, sizeof(double)*numLevels);
    double minVal=v[0];
    double maxVal=v[0];
    int npix=dims[0]*dims[1];
    for(int i=0;i<npix;++i){
        if(v[i]<minVal){
            minVal=v[i];
        }else if(v[i]>maxVal){
            maxVal=v[i];
        }
    }
    const double epsilon=1e-8;
    double delta=(maxVal-minVal+epsilon)/numLevels;
    if((numLevels<2)||(delta<epsilon)){
        memset(out, 0, sizeof(double)*npix);
        levels[0]=0.5*(minVal+maxVal);
        hist[0]=npix;
        return;
    }
    levels[0]=delta*0.5;
    for(int i=1;i<numLevels;++i){
        levels[i]=levels[i-1]+delta;
    }
    for(int i=0;i<npix;++i){
        int l=(v[i]-minVal)/delta;
        out[i]=l;
        hist[l]++;
    }
}

void quantizePositiveImageCPP(double *v, int *dims, int numLevels, int *out, double *levels, int *hist){
    --numLevels;//zero is one of the levels
    if(numLevels<1){
        return;
    }
    memset(hist, 0, sizeof(int)*(numLevels+1));
    memset(levels, 0, sizeof(double)*(numLevels+1));
    double minVal=-1;
    double maxVal=-1;
    int npix=dims[0]*dims[1];
    for(int i=0;i<npix;++i)if(v[i]>0){
        if((minVal<0) || (v[i]<minVal)){
            minVal=v[i];
        }
        if(v[i]>maxVal){
            maxVal=v[i];
        }
    }
    const double epsilon=1e-8;
    double delta=(maxVal-minVal+epsilon)/numLevels;
    if((numLevels<2)||(delta<epsilon)){
        for(int i=0;i<npix;++i){
            if(v[i]>0){
                out[i]=1;
            }else{
                out[i]=0;
                ++hist[0];
            }
        }
        levels[0]=0;
        levels[1]=0.5*(minVal+maxVal);
        hist[1]=npix-hist[0];
        return;
    }
    levels[0]=0;
    levels[1]=delta*0.5;
    for(int i=2;i<=numLevels;++i){
        levels[i]=levels[i-1]+delta;
    }
    for(int i=0;i<npix;++i){
        if(v[i]>0){
            int l=(v[i]-minVal)/delta;
            out[i]=l+1;
            hist[l+1]++;
        }else{
            out[i]=0;
            hist[0]++;
        }
    }
}



void quantizeVolumeCPP(double *v, int *dims, int numLevels, int *out, double *levels, int *hist){
    if(numLevels<1){
        return;
    }
    memset(hist, 0, sizeof(int)*numLevels);
    memset(levels, 0, sizeof(double)*numLevels);
    double minVal=v[0];
    double maxVal=v[0];
    int nvox=dims[0]*dims[1]*dims[2];
    for(int i=0;i<nvox;++i){
        if(v[i]<minVal){
            minVal=v[i];
        }else if(v[i]>maxVal){
            maxVal=v[i];
        }
    }
    const double epsilon=1e-8;
    double delta=(maxVal-minVal+epsilon)/numLevels;
    if((numLevels<2)||(delta<epsilon)){
        memset(out, 0, sizeof(double)*nvox);
        levels[0]=0.5*(minVal+maxVal);
        hist[0]=nvox;
        return;
    }
    levels[0]=delta*0.5;
    for(int i=1;i<numLevels;++i){
        levels[i]=levels[i-1]+delta;
    }
    for(int i=0;i<nvox;++i){
        int l=(v[i]-minVal)/delta;
        out[i]=l;
        hist[l]++;
    }
}

void quantizePositiveVolumeCPP(double *v, int *dims, int numLevels, int *out, double *levels, int *hist){
    --numLevels;//zero is one of the levels
    if(numLevels<1){
        return;
    }
    memset(hist, 0, sizeof(int)*(numLevels+1));
    memset(levels, 0, sizeof(double)*(numLevels+1));
    double minVal=-1;
    double maxVal=-1;
    int nvox=dims[0]*dims[1]*dims[2];
    for(int i=0;i<nvox;++i)if(v[i]>0){
        if((minVal<0) || (v[i]<minVal)){
            minVal=v[i];
        }
        if(v[i]>maxVal){
            maxVal=v[i];
        }
    }
    const double epsilon=1e-8;
    double delta=(maxVal-minVal+epsilon)/numLevels;
    if((numLevels<2)||(delta<epsilon)){
        for(int i=0;i<nvox;++i){
            if(v[i]>0){
                out[i]=1;
            }else{
                out[i]=0;
                ++hist[0];
            }
        }
        levels[0]=0;
        levels[1]=0.5*(minVal+maxVal);
        hist[1]=nvox-hist[0];
        return;
    }
    levels[0]=0;
    levels[1]=delta*0.5;
    for(int i=2;i<=numLevels;++i){
        levels[i]=levels[i-1]+delta;
    }
    for(int i=0;i<nvox;++i){
        if(v[i]>0){
            int l=(v[i]-minVal)/delta;
            out[i]=l+1;
            hist[l+1]++;
        }else{
            out[i]=0;
            hist[0]++;
        }
    }
}

void computeImageClassStatsCPP(double *v, int *dims, int numLabels, int *labels, double *means, double *variances){
    memset(means, 0, sizeof(double)*numLabels);
    memset(variances, 0, sizeof(double)*numLabels);
    int *counts=new int[numLabels];
    memset(counts, 0, sizeof(int)*numLabels);
    int numPixels=dims[0]*dims[1];
    for(int i=0;i<numPixels;++i){
        means[labels[i]]+=v[i];
        variances[labels[i]]+=v[i]*v[i];
        counts[labels[i]]++;
    }
    for(int i=0;i<numLabels;++i){
        if(counts[i]>0){
            means[i]/=counts[i];
        }
        if(counts[i]>1){
            variances[i]=variances[i]/counts[i]-means[i]*means[i];
        }else{
            variances[i]=INF64;
        }
    }
    delete[] counts;
}

void computeMaskedImageClassStatsCPP(int *mask, double *v, int *dims, int numLabels, int *labels, double *means, double *variances){
    memset(means, 0, sizeof(double)*numLabels);
    memset(variances, 0, sizeof(double)*numLabels);
    int *counts=new int[numLabels];
    memset(counts, 0, sizeof(int)*numLabels);
    int numPixels=dims[0]*dims[1];
    for(int i=0;i<numPixels;++i)if(mask[i]!=0){
        means[labels[i]]+=v[i];
        variances[labels[i]]+=v[i]*v[i];
        counts[labels[i]]++;
    }
    for(int i=0;i<numLabels;++i){
        if(counts[i]>0){
            means[i]/=counts[i];
        }
        if(counts[i]>1){
            variances[i]=variances[i]/counts[i]-means[i]*means[i];
        }else{
            variances[i]=INF64;
        }
    }
    delete[] counts;
}

void computeVolumeClassStatsCPP(double *v, int *dims, int numLabels, int *labels, double *means, double *variances){
    memset(means, 0, sizeof(double)*numLabels);
    memset(variances, 0, sizeof(double)*numLabels);
    int *counts=new int[numLabels];
    memset(counts, 0, sizeof(int)*numLabels);
    int numVoxels=dims[0]*dims[1]*dims[2];
    for(int i=0;i<numVoxels;++i){
        means[labels[i]]+=v[i];
        variances[labels[i]]+=v[i]*v[i];
        counts[labels[i]]++;
    }
    for(int i=0;i<numLabels;++i){
        if(counts[i]>0){
            means[i]/=counts[i];
        }
        if(counts[i]>1){
            variances[i]=variances[i]/counts[i]-means[i]*means[i];
        }else{
            variances[i]=INF64;
        }
    }
    delete[] counts;
}

void computeMaskedVolumeClassStatsCPP(int *mask, double *v, int *dims, int numLabels, int *labels, double *means, double *variances){
    memset(means, 0, sizeof(double)*numLabels);
    memset(variances, 0, sizeof(double)*numLabels);
    int *counts=new int[numLabels];
    memset(counts, 0, sizeof(int)*numLabels);
    int numVoxels=dims[0]*dims[1]*dims[2];
    for(int i=0;i<numVoxels;++i)if(mask[i]!=0){
        means[labels[i]]+=v[i];
        variances[labels[i]]+=v[i]*v[i];
        counts[labels[i]]++;
    }
    for(int i=0;i<numLabels;++i){
        if(counts[i]>0){
            means[i]/=counts[i];
        }
        if(counts[i]>1){
            variances[i]=variances[i]/counts[i]-means[i]*means[i];
        }else{
            variances[i]=INF64;
        }
    }
    delete[] counts;
}


void integrateWeightedTensorFieldProductsCPP(double *q, int *dims, double *diff, int numLabels, int *labels, double *weights, double *Aw, double *bw){
    int k=dims[3];
    int nvox=dims[0]*dims[1]*dims[2];
    double *AA=new double[k*k*numLabels];
    double *bb=new double[k*numLabels];
    memset(AA, 0, sizeof(double)*k*k*numLabels);
    memset(bb, 0, sizeof(double)*k*numLabels);
    double *qq=q;
    for(int pos=0;pos<nvox;++pos, qq+=k){
        int idx=labels[pos];
        double *A=&AA[k*k*idx];
        double *b=&bb[k*idx];
        for(int i=0;i<k;++i){
            b[i]+=qq[i]*diff[pos];
            for(int j=0;j<k;++j){
                A[k*i+j]+=qq[i]*qq[j];
            }
        }
    }
    memset(Aw, 0, sizeof(double)*k*k);
    memset(bw, 0, sizeof(double)*k);
    double *A=AA;
    double *b=bb;
    for(int c=0;c<numLabels;++c, A+=(k*k), b+=k){
        for(int i=0;i<k;++i){
            bw[i]+=weights[c]*b[i];
            for(int j=0;j<k;++j){
                Aw[i*k+j]+=weights[c]*A[i*k+j];
            }
        }
    }
    delete[] AA;
    delete[] bb;
}

void integrateMaskedWeightedTensorFieldProductsCPP(int *mask, double *q, int *dims, double *diff, int numLabels, int *labels, double *weights, double *Aw, double *bw){
    int k=dims[3];
    int nvox=dims[0]*dims[1]*dims[2];
    double *AA=new double[k*k*numLabels];
    double *bb=new double[k*numLabels];
    memset(AA, 0, sizeof(double)*k*k*numLabels);
    memset(bb, 0, sizeof(double)*k*numLabels);
    double *qq=q;
    for(int pos=0;pos<nvox;++pos, qq+=k)if(mask[pos]!=0){
        int idx=labels[pos];
        double *A=&AA[k*k*idx];
        double *b=&bb[k*idx];
        for(int i=0;i<k;++i){
            b[i]+=qq[i]*diff[pos];
            for(int j=0;j<k;++j){
                A[k*i+j]+=qq[i]*qq[j];
            }
        }
    }
    memset(Aw, 0, sizeof(double)*k*k);
    memset(bw, 0, sizeof(double)*k);
    double *A=AA;
    double *b=bb;
    for(int c=0;c<numLabels;++c, A+=(k*k), b+=k){
        for(int i=0;i<k;++i){
            bw[i]+=weights[c]*b[i];
            for(int j=0;j<k;++j){
                Aw[i*k+j]+=weights[c]*A[i*k+j];
            }
        }
    }
    delete[] AA;
    delete[] bb;
}

void solve2DSymmetricPositiveDefiniteSystem(double *A, double *y, double *x, double *residual){
    double den=(A[0]*A[2]-A[1]*A[1]);
    x[1]=(A[0]*y[1]-A[1]*y[0])/den;
    x[0]=(y[0]-A[1]*x[1])/A[0];
    if(residual!=NULL){
        double r0=A[0]*x[0]+A[1]*x[1] - y[0];
        double r1=A[1]*x[0]+A[2]*x[1] - y[1];
        *residual=sqrt(r0*r0+r1*r1);
    }
}

void solve3DSymmetricPositiveDefiniteSystem(double *A, double *y, double *x, double *residual){
    double a=A[0], b=A[1], c=A[2];
    double d=(a*A[3]-b*b)/a;
    double e=(a*A[4]-b*c)/a;
    double f=(a*A[5]-c*c)/a - (e*e*a)/(a*A[3]-b*b);
    double y0=y[0];
    double y1=(y[1]*a-y0*b)/a;
    double y2=(y[2]*a-A[2]*y0)/a - (e*(y[1]*a-b*y0))/(a*A[3]-b*b);
    x[2]=y2/f;
    x[1]=(y1-e*x[2])/d;
    x[0]=(y0-b*x[1]-c*x[2])/a;
    if(residual!=NULL){
        double r0=A[0]*x[0]+A[1]*x[1]+A[2]*x[2] - y[0];
        double r1=A[1]*x[0]+A[3]*x[1]+A[4]*x[2] - y[1];
        double r2=A[2]*x[0]+A[4]*x[1]+A[5]*x[2] - y[2];
        *residual=sqrt(r0*r0+r1*r1+r2*r2);
    }
}


//#define GLOBAL_FIELD_CORRECTION
double iterateDisplacementField2DCPP(double *deltaField, double *sigmaField, double *gradientField, int *dims, double lambdaParam, double *previousDisplacement, double *displacementField, double *residual){
    const static int numNeighbors=4;
    const static int dRow[]={-1, 0, 1,  0, -1, 1,  1, -1};
    const static int dCol[]={ 0, 1, 0, -1,  1, 1, -1, -1};
    int nrows=dims[0];
    int ncols=dims[1];
    double *d=displacementField;
    double *prevd=previousDisplacement;
    double *g=gradientField;
    double y[2];
    double A[3];
    int pos=0;
    double maxDisplacement=0;
    for(int r=0;r<nrows;++r){
        for(int c=0;c<ncols;++c, ++pos, d+=2, prevd+=2, g+=2){
            double delta=deltaField[pos];
            double sigma=sigmaField[pos];
            int nn=0;
            y[0]=y[1]=0;
            for(int k=0;k<numNeighbors;++k){
                int dr=r+dRow[k];
                if((dr<0) || (dr>=nrows)){
                    continue;
                }
                int dc=c+dCol[k];
                if((dc<0) || (dc>=ncols)){
                    continue;
                }
                ++nn;
                double *dneigh=&displacementField[2*(dr*ncols + dc)];
#ifdef GLOBAL_FIELD_CORRECTION
                double *prevNeigh=&previousDisplacement[2*(dr*ncols + dc)];
                y[0]+=dneigh[0]+prevNeigh[0];
                y[1]+=dneigh[1]+prevNeigh[1];
#else
                y[0]+=dneigh[0];
                y[1]+=dneigh[1];
#endif
            }
            if(isInfinite(sigma)){
                double xx=d[0];
                double yy=d[1];
                d[0]=y[0]/nn;
                d[1]=y[1]/nn;
                xx-=d[0];
                yy-=d[1];
                double opt=xx*xx+yy*yy;
                if(maxDisplacement<opt){
                    maxDisplacement=opt;
                }
                residual[pos]=0;
            }else{
#ifdef GLOBAL_FIELD_CORRECTION
                y[0]=(delta*g[0]) + sigma*lambdaParam*(y[0]-nn*prevd[0]);
                y[1]=(delta*g[1]) + sigma*lambdaParam*(y[1]-nn*prevd[1]);
#else
                y[0]=(delta*g[0]) + sigma*lambdaParam*y[0];
                y[1]=(delta*g[1]) + sigma*lambdaParam*y[1];
#endif
                A[0]=g[0]*g[0] + sigma*lambdaParam*(nn);
                A[1]=g[0]*g[1];
                A[2]=g[1]*g[1] + sigma*lambdaParam*(nn);
                double xx=d[0];
                double yy=d[1];
                solve2DSymmetricPositiveDefiniteSystem(A,y,d, &residual[pos]);
                xx-=d[0];
                yy-=d[1];
                double opt=xx*xx+yy*yy;
                if(maxDisplacement<opt){
                    maxDisplacement=opt;
                }
            }//if
        }//cols
    }//rows
    return sqrt(maxDisplacement);
}

double iterateMaskedDisplacementField2DCPP(double *deltaField, double *sigmaField, double *gradientField, int *mask, int *dims, double lambdaParam, double *previousDisplacement, double *displacementField, double *residual){
    const static int numNeighbors=4;
    const static int dRow[]={-1, 0, 1,  0, -1, 1,  1, -1};
    const static int dCol[]={ 0, 1, 0, -1,  1, 1, -1, -1};
    int nrows=dims[0];
    int ncols=dims[1];
    double *d=displacementField;
    double *prevd=previousDisplacement;
    double *g=gradientField;
    double y[2];
    double A[3];
    int pos=0;
    double maxDisplacement=0;
    int *mm=mask;
    for(int r=0;r<nrows;++r){
        for(int c=0;c<ncols;++c, ++pos, d+=2, prevd+=2, g+=2, ++mm){
            if(!*mm){
                d[0]=d[1]=0;
                continue;
            }
            double delta=deltaField[pos];
            double sigma=sigmaField[pos];
            int nn=0;
            y[0]=y[1]=0;
            for(int k=0;k<numNeighbors;++k){
                int dr=r+dRow[k];
                if((dr<0) || (dr>=nrows)){
                    continue;
                }
                int dc=c+dCol[k];
                if((dc<0) || (dc>=ncols)){
                    continue;
                }
                if(mask[dr*ncols+dc]==0){
                    continue;
                }
                ++nn;
                double *dneigh=&displacementField[2*(dr*ncols + dc)];
#ifdef GLOBAL_FIELD_CORRECTION
                double *prevNeigh=&previousDisplacement[2*(dr*ncols + dc)];
                y[0]+=dneigh[0]+prevNeigh[0];
                y[1]+=dneigh[1]+prevNeigh[1];
#else
                y[0]+=dneigh[0];
                y[1]+=dneigh[1];
#endif
            }
            if(nn<2){
                d[0]=d[1]=0;
                continue;
            }
            if(isInfinite(sigma)){
                double xx=d[0];
                double yy=d[1];
                d[0]=y[0]/nn;
                d[1]=y[1]/nn;
                xx-=d[0];
                yy-=d[1];
                double opt=xx*xx+yy*yy;
                if(maxDisplacement<opt){
                    maxDisplacement=opt;
                }
                residual[pos]=0;
            }else{
#ifdef GLOBAL_FIELD_CORRECTION
                y[0]=(delta*g[0]) + sigma*lambdaParam*(y[0]-nn*prevd[0]);
                y[1]=(delta*g[1]) + sigma*lambdaParam*(y[1]-nn*prevd[1]);
#else
                y[0]=(delta*g[0]) + sigma*lambdaParam*y[0];
                y[1]=(delta*g[1]) + sigma*lambdaParam*y[1];
#endif
                A[0]=g[0]*g[0] + sigma*lambdaParam*(nn);
                A[1]=g[0]*g[1];
                A[2]=g[1]*g[1] + sigma*lambdaParam*(nn);
                double xx=d[0];
                double yy=d[1];
                solve2DSymmetricPositiveDefiniteSystem(A,y,d, &residual[pos]);
                xx-=d[0];
                yy-=d[1];
                double opt=xx*xx+yy*yy;
                if(maxDisplacement<opt){
                    maxDisplacement=opt;
                }
            }//if
        }//cols
    }//rows
    return sqrt(maxDisplacement);
}


double iterateDisplacementField3DCPP(double *deltaField, double *sigmaField, double *gradientField, int *dims, double lambdaParam, double *previousDisplacement, double *displacementField, double *residual){
    const static int numNeighbors=6;
    const static int dSlice[numNeighbors]={-1,  0, 0, 0,  0, 1};
    const static int dRow[numNeighbors]  ={ 0, -1, 0, 1,  0, 0};
    const static int dCol[numNeighbors]  ={ 0,  0, 1, 0, -1, 0};
    int nslices=dims[0];
    int nrows=dims[1];
    int ncols=dims[2];
    double *d=displacementField;
    double *prevd=previousDisplacement;
    double *g=gradientField;
    int sliceSize=ncols*nrows;
    double y[3];
    double A[6];
    int pos=0;
    double maxDisplacement=0;
    for(int s=0;s<nslices;++s){
        for(int r=0;r<nrows;++r){
            for(int c=0;c<ncols;++c, ++pos, d+=3, prevd+=3, g+=3){
                double delta=deltaField[pos];
                double sigma=sigmaField[pos];
                int nn=0;
                y[0]=y[1]=y[2]=0;
                for(int k=0;k<numNeighbors;++k){
                    int ds=s+dSlice[k];
                    if((ds<0) || (ds>=nslices)){
                        continue;
                    }
                    int dr=r+dRow[k];
                    if((dr<0) || (dr>=nrows)){
                        continue;
                    }
                    int dc=c+dCol[k];
                    if((dc<0) || (dc>=ncols)){
                        continue;
                    }
                    ++nn;
                    double *dneigh=&displacementField[3*(ds*sliceSize + dr*ncols + dc)];
#ifdef GLOBAL_FIELD_CORRECTION
                    double *prevNeigh=&previousDisplacement[3*(ds*sliceSize + dr*ncols + dc)];
                    y[0]+=dneigh[0]+prevNeigh[0];
                    y[1]+=dneigh[1]+prevNeigh[1];
                    y[2]+=dneigh[2]+prevNeigh[2];
#else
                    y[0]+=dneigh[0];
                    y[1]+=dneigh[1];
                    y[2]+=dneigh[2];
#endif
                }
                if(isInfinite(sigma)){
                    double xx=d[0];
                    double yy=d[1];
                    double zz=d[2];
                    d[0]=y[0]/nn;
                    d[1]=y[1]/nn;
                    d[2]=y[2]/nn;
                    xx-=d[0];
                    yy-=d[1];
                    zz-=d[2];
                    double opt=xx*xx+yy*yy+zz*zz;
                    if(maxDisplacement<opt){
                        maxDisplacement=opt;
                    }
                    residual[pos]=0;
                }else{
#ifdef GLOBAL_FIELD_CORRECTION
                    y[0]=(delta*g[0]) + sigma*lambdaParam*(y[0]-nn*prevd[0]);
                    y[1]=(delta*g[1]) + sigma*lambdaParam*(y[1]-nn*prevd[1]);
                    y[1]=(delta*g[2]) + sigma*lambdaParam*(y[2]-nn*prevd[2]);
#else
                    y[0]=(delta*g[0]) + sigma*lambdaParam*y[0];
                    y[1]=(delta*g[1]) + sigma*lambdaParam*y[1];
                    y[2]=(delta*g[2]) + sigma*lambdaParam*y[2];
#endif
                    A[0]=g[0]*g[0] + sigma*lambdaParam*nn;
                    A[1]=g[0]*g[1];
                    A[2]=g[0]*g[2];
                    A[3]=g[1]*g[1] + sigma*lambdaParam*nn;
                    A[4]=g[1]*g[2];
                    A[5]=g[2]*g[2] + sigma*lambdaParam*nn;
                    double xx=d[0];
                    double yy=d[1];
                    double zz=d[2];
                    solve3DSymmetricPositiveDefiniteSystem(A,y,d, &residual[pos]);
                    xx-=d[0];
                    yy-=d[1];
                    zz-=d[2];
                    double opt=xx*xx+yy*yy+zz*zz;
                    if(maxDisplacement<opt){
                        maxDisplacement=opt;
                    }
                }
            }//cols
        }//rows
    }//slices
    return sqrt(maxDisplacement);
}





void computeMaskedVolumeClassStatsProbsCPP(int *mask, double *img, int *dims, int nclasses, double *probs, double *means, double *variances){
    const double EPSILON=1e-9;
    memset(means, 0, sizeof(double)*nclasses);
    memset(variances, 0, sizeof(double)*nclasses);
    double *sums=new double[nclasses];
    memset(sums, 0, sizeof(double)*nclasses);
    int nsites=dims[0]*dims[1];
    //---means---
    double *p=probs;
    for(int i=0;i<nsites;++i, p+=nclasses)if(mask[i]!=0){
        for(int k=0;k<nclasses;++k){
            double p2=p[k]*p[k];
            means[k]+=img[i]*p2;
            sums[k]+=p2;
        }
    }
    for(int k=0;k<nclasses;++k){
        if(sums[k]>EPSILON){
            means[k]/=sums[k];
        }
    }
    //---variances---
    p=probs;
    for(int i=0;i<nsites;++i, p+=nclasses)if(mask[i]!=0){
        for(int k=0;k<nclasses;++k){
            double p2=p[k]*p[k];
            variances[k]+=(img[i]-means[k])*(img[i]-means[k])*p2;
        }
    }
    for(int k=0;k<nclasses;++k){
        if(sums[k]>EPSILON){
            variances[k]/=sums[k];
        }else{
            variances[k]=INF64;
        }
    }
    delete[] sums;
}


void integrateMaskedWeightedTensorFieldProductsProbsCPP(int *mask, double *q, int *dims, double *diff, int nclasses, double *probs, double *weights, double *Aw, double *bw){
    int k=dims[2];
    int nsites=dims[0]*dims[1];
    double *AA=new double[k*k*nclasses];
    double *bb=new double[k*nclasses];
    double *sums=new double[nclasses];
    memset(AA, 0, sizeof(double)*k*k*nclasses);
    memset(bb, 0, sizeof(double)*k*nclasses);
    memset(sums, 0, sizeof(double)*nclasses);
    double *qq=q;
    double *p=probs;
    //--------TO-DO: how to handle the p^2's ? --
    for(int pos=0;pos<nsites;++pos, qq+=k, p+=nclasses)if(mask[pos]!=0){
        for(int idx=0;idx<nclasses;++idx){
            double *A=&AA[k*k*idx];
            double *b=&bb[k*idx];
            double p2=p[idx]*p[idx];
            sums[idx]+=p2;
            for(int i=0;i<k;++i){
                b[i]+=p2*qq[i]*diff[pos];
                for(int j=0;j<k;++j){
                    A[k*i+j]+=p2*qq[i]*qq[j];
                }
            }
        }
    }
    memset(Aw, 0, sizeof(double)*k*k);
    memset(bw, 0, sizeof(double)*k);
    double *A=AA;
    double *b=bb;
    for(int c=0;c<nclasses;++c, A+=(k*k), b+=k){
        for(int i=0;i<k;++i){
            bw[i]+=weights[c]*b[i];
            for(int j=0;j<k;++j){
                Aw[i*k+j]+=weights[c]*A[i*k+j];
            }
        }
    }
    delete[] AA;
    delete[] bb;
    delete[] sums;
}

double computeInverseEnergy(double *d, double *invd, int nrows, int ncols, double lambda){
    double stats[3];
    double *residual=new double[2*nrows*ncols];
    composeVectorFields(d, invd, nrows, ncols, residual, stats);
    double energy=0;
    for(int i=0;i<nrows-1;++i){
        for(int j=0;j<ncols-1;++j){
            double d00=invd[2*(i*nrows+j)] - invd[2*((i+1)*nrows+j)];
            double d01=invd[2*(i*nrows+j)+1] - invd[2*((i+1)*nrows+j)+1];
            double d10=invd[2*(i*nrows+j)] - invd[2*(i*nrows+j+1)];
            double d11=invd[2*(i*nrows+j)+1] - invd[2*(i*nrows+j+1)+1];
            energy+=d00*d00+d01*d01+d10*d10+d11*d11;
        }    
    }
    energy*=lambda;
    double *r=residual;
    for(int i=0;i<nrows;++i){
        for(int j=0;j<ncols;++j, r+=2){
            energy+=r[0]+r[0]+r[1]*r[1];
        }    
    }
    delete[] residual;
    return energy;
    
}

int invertVectorField(double *forward, int nrows, int ncols, double lambdaParam, int maxIter, double tolerance, double *inv, double *stats){
    const static int numNeighbors=4;
    const static int dRow[]={-1, 0, 1,  0, -1, 1,  1, -1};
    const static int dCol[]={ 0, 1, 0, -1,  1, 1, -1, -1};
    const static int nRow[]={ 0, 0, 1, 1};
    const static int nCol[]={ 0, 1, 0, 1};
    double gamma[4];
    double gamma2[4];
    double *temp=new double[nrows*ncols*2];
    double *residual=new double[nrows*ncols*2];
    double *denom=new double[nrows*ncols];
    memset(inv, 0, sizeof(double)*nrows*ncols*2);
    double maxChange=tolerance+1;
    int iter;
    double substats[3];
    for(iter=0;(tolerance*tolerance<maxChange)&&(iter<maxIter);++iter){
        memset(temp, 0, sizeof(double)*nrows*ncols*2);
        memset(denom, 0, sizeof(double)*nrows*ncols);
        //---interpolate the current approximation and accumulate with the forward field---
        composeVectorFields(forward, inv, nrows, ncols, residual, substats);
        double *r=residual;
        double *f=forward;
        for(int i=0;i<nrows;++i){
            for(int j=0;j<ncols;++j,r+=2, f+=2){
                //--accumulate all regularization terms--
                for(int k=0;k<numNeighbors;++k){
                    int ii=i+dRow[k];
                    if((ii<0)||(ii>=nrows)){
                        continue;
                    }
                    int jj=j+dCol[k];
                    if((jj<0)||(jj>=ncols)){
                        continue;
                    }
                    denom[i*ncols+j]+=lambdaParam;
                    temp[2*(i*ncols+j)]+=lambdaParam*inv[2*(ii*ncols+jj)];
                    temp[2*(i*ncols+j)+1]+=lambdaParam*inv[2*(ii*ncols+jj)+1];
                }
                //find the variables that are affected by this input point, and their interpolation coefficients
                double dii=i+f[0];
                double djj=j+f[1];
                if((dii<0) || (djj<0) || (dii>nrows-1)||(djj>ncols-1)){//no one is affected
                    continue;
                }
                //find the top left index and the interpolation coefficients
                int ii=floor(f[0]);
                int jj=floor(f[1]);
                double calpha=f[0]-ii;
                double cbeta=f[1]-jj;
                ii+=i;
                jj+=j;
                double alpha=1-calpha;
                double beta=1-cbeta;
                //---finally accumulate the affected terms---
                gamma[0]=alpha*beta;//top left
                gamma[1]=alpha*cbeta;//top right
                gamma[2]=calpha*beta;//bottom left
                gamma[3]=calpha*cbeta;//bottom right
                for(int k=0;k<4;++k){
                    int iii=ii+nRow[k];
                    int jjj=jj+nCol[k];
                    if((iii<0)||(jjj<0)||(iii>=nrows)||(jjj>=ncols)){
                        continue;
                    }
                    gamma2[k]=gamma[k]*gamma[k];
                    temp[2*(iii*ncols+jjj)]+=inv[2*(iii*ncols+jjj)]*gamma2[k] - r[0]*gamma[k];
                    temp[2*(iii*ncols+jjj)+1]+=inv[2*(iii*ncols+jjj)+1]*gamma2[k] - r[1]*gamma[k];
                    denom[iii*ncols+jjj]+=gamma2[k];
                }//for k
            }//for j
        }//for i
        //now execute the Jacobi iteration
        double *id=inv;
        double *tmp=temp;
        double *den=denom;
        maxChange=0;
        for(int i=0;i<nrows;++i){
            for(int j=0;j<ncols;++j, id+=2, tmp+=2, den++){
                tmp[0]/=(*den);
                tmp[1]/=(*den);
                double nrm=(tmp[0]-id[0])*(tmp[0]-id[0])+(tmp[1]-id[1])*(tmp[1]-id[1]);
                if(maxChange<nrm){
                    maxChange=nrm;
                }
                id[0]=tmp[0];
                id[1]=tmp[1];
            }
        }
    }//for iter
    delete[] temp;
    delete[] denom;
    delete[] residual;
    stats[0]=sqrt(maxChange);
    stats[1]=iter;
    return 0;
}
//==================================================================================================
//==================================================================================================
//=====================================Yan's algorithm begins=======================================
//==================================================================================================
//==================================================================================================
//==================================================================================================
inline bool checkIsFinite(double *error, int nrows, int ncols, int r, int c){
    if((r<0)||(c<0)||(r>=nrows)||(c>=ncols)){
        return false;
    }
    if(isInfinite(error[r*ncols+c])){
        return false;
    }
    return true;
}

void findNearesFinite(double *error, int nrows, int ncols, int r, int c, int &rr, int &cc){
    int ring=1;
    while(true){
        rr=r-ring-1;
        cc=c-1;
        for(int k=0;k<=ring;++k){//top-right side of the diamond
            ++rr;
            ++cc;
            if(checkIsFinite(error, nrows, ncols, rr, cc)){
                return;
            }
        }
        for(int k=0;k<ring;++k){//bottom-right side of the diamond
            ++rr;
            --cc;
            if(checkIsFinite(error, nrows, ncols, rr, cc)){
                return;
            }
        }
        for(int k=0;k<ring;++k){//bottom-left side of the diamond
            --rr;
            --cc;
            if(checkIsFinite(error, nrows, ncols, rr, cc)){
                return;
            }
        }
        for(int k=0;k<ring-1;++k){//top-left side of the diamond
            --rr;
            ++cc;
            if(checkIsFinite(error, nrows, ncols, rr, cc)){
                return;
            }
        }
        ++ring;
    }

}

void interpolateDisplacementFieldAt(double *forward, int nrows, int ncols, double pr, double pc, double &fr, double &fc){
    if((pr<0) || (pc<0) || (pr>nrows-1) || (pc>ncols-1)){
        fr=0;
        fc=0;
        return;
    }
    int i=floor(pr);
    int j=floor(pc);
    double alphac=pr-i;
    double betac=pc-j;
    double alpha=1.0-alphac;
    double beta=1.0-betac;
    double *ff=&forward[2*(i*ncols+j)];
    fr=alpha*beta*ff[0];
    fc=alpha*beta*ff[1];
    ++j;
    if(j<ncols){
        ff=&forward[2*(i*ncols+j)];
        fr+=alpha*betac*ff[0];
        fc+=alpha*betac*ff[1];
    }
    ++i;
    if((i<nrows)&&(j<ncols)){
        ff=&forward[2*(i*ncols+j)];
        fr+=alphac*betac*ff[0];
        fc+=alphac*betac*ff[1];
    }
    --j;
    if(i<nrows){
        ff=&forward[2*(i*ncols+j)];
        fr+=alphac*beta*ff[0];
        fc+=alphac*beta*ff[1];
    }
}

int invertVectorFieldYan(double *forward, int nrows, int ncols, int maxloop, double tolerance, double *inv){
    const int numNeighbors=4;
    const static int dRow[]={-1, 0, 1,  0, -1, 1,  1, -1};
    const static int dCol[]={ 0, 1, 0, -1,  1, 1, -1, -1};
    const static int nRow[]={ 0, 0, 1, 1};
    const static int nCol[]={ 0, 1, 0, 1};
    // Step 0: initialize delta, MAXLOOP and inverse computing error
    double *error=new double[nrows*ncols];
    for(int i=nrows*ncols-1;i>=0;--i){
        error[i]=INF64;
    }
    //Step 1a: Map points from p in R, assign e(q) for points q in S which are immediately adjacent to f(p) if assignment reduces e(q)
    double *f=forward;
    for(int i=0;i<nrows;++i){
        for(int j=0;j<ncols;++j, f+=2){//p=(i,j) in R
            double dii=i+f[0];
            double djj=j+f[1];//(dii, djj) = f(p)
            if((dii<0) || (djj<0) || (dii>nrows-1)||(djj>ncols-1)){//no one is affected
                continue;
            }
            //find the top left index and the interpolation coefficients
            int ii=floor(dii);
            int jj=floor(djj);
            //assign all grid points immediately adjacent to f(p)
            for(int k=0;k<4;++k){
                int iii=ii+nRow[k];
                int jjj=jj+nCol[k];//(iii, jjj)=q is a surrounding point
                if((iii<0)||(jjj<0)||(iii>=nrows)||(jjj>=ncols)){
                    continue;//the point is outside the lattice
                }
                double dr=dii-iii;
                double dc=djj-jjj;//(dr,dc) = f(p) - q
                double opt=sqrt(dr*dr+dc*dc);//||q-f(p)||
                if(opt<error[iii*ncols+jjj]){//if(||q-f(p)||^2 < e(q))
                    double *dq=&inv[2*(iii*ncols+jjj)];
                    dq[0]=i-iii;
                    dq[1]=j-jjj;//g(q)=p  <<==>>  q+inv[q] = p <<==>> inv[q]=p-q
                    error[iii*ncols+jjj]=opt;
                }
            }
        }
    }
    // Step 1b: map unmapped points in S via nearest neighbor
    double *dq=inv;
    for(int i=0;i<nrows;++i){
        for(int j=0;j<ncols;++j,dq+=2){//q=(i,j)
            if(!isInfinite(error[i*ncols+j])){
                continue;
            }
            //find nearest neighbor q’ in S with finite e(q’)
            int ii,jj;
            findNearesFinite(error, nrows, ncols, i, j, ii, jj);//(ii, jj)=q'
            double *dqprime=&inv[2*(ii*ncols+jj)];
            dq[0]=ii+dqprime[0]-i;
            dq[1]=jj+dqprime[1]-j;//g(q)=g(q') <<==>> q+inv[q] = q'+inv[q']  <<==>>  inv[q]=q'+inv[q']-q
            double fr,fc;
            interpolateDisplacementFieldAt(forward, nrows, ncols, ii+dqprime[0], jj+dqprime[1], fr, fc);//(ii+dqprime[0], jj+dqprime[1])+(fr,fc) = f(g(q')) 
            double dr=ii+dqprime[0]+fr-i;
            double dc=jj+dqprime[1]+fc-j;//(dr, dc)=f(g(q'))-q
            error[i*ncols+j]=sqrt(dr*dr+dc*dc);
        }
    }
    // Step 2a: Search around points with e(q) > tolerance in S
    double *e=error;
    dq=inv;
    for(int i=0;i<nrows;++i){
        for(int j=0;j<ncols;++j,++e, dq+=2){//q=(i,j) in S
            double stepSize=*e;
            double cr=i+dq[0];
            double cc=j+dq[1];//(cr,cc) = g(q)
            int iter=0;
            while((*e > tolerance)&&(iter<maxloop)){
                for(int signRow=-1;signRow<=1;signRow+=2){
                    for(int signCol=-1;signCol<=1;signCol+=2){
                        double vr=cr+signRow*stepSize*0.5;
                        double vc=cc+signCol*stepSize*0.5;// p'=(vr,vc) (corner of he box centered at g(q))
                        double fvr, fvc;
                        interpolateDisplacementFieldAt(forward, nrows, ncols, vr, vc, fvr, fvc);//f(p') = (vr+fvr, vc+fvc)
                        double dr=i-vr-fvr;
                        double dc=j-vc-fvc;//q-f(p') = (i,j) - (vr+fvr, vc+fvc)
                        double opt=sqrt(dr*dr+dc*dc);
                        if(opt<*e){
                            dq[0]=vr-i;
                            dq[1]=vc-j;//g(q)=p'  <<==>> q+inv[q] = p'  <<==>> inv[q] = p'-q
                            *e=opt;
                        }
                    }
                }
                ++iter;
                stepSize*=0.5;
            }
        }
    }
    // Step 2b: Systematic search around active grid points (adjacent points with low errors) instead of around the points themselves
    bool finished=false;
    int iter=0;
    while((!finished) && (iter<maxloop)){
        ++iter;
        e=error;
        dq=inv;
        for(int i=0;i<nrows;++i){
            for(int j=0;j<ncols;++j,++e, dq+=2){//q=(i,j)
                if(*e>=tolerance){
                    continue;
                }
                //now error[q]<tolerance
                for(int k=0;k<numNeighbors;++k){//look for adjacent vertices
                    int ii=i+dRow[k];
                    int jj=j+dCol[k];
                    if((ii<0)||(jj<0)||(i>=nrows)||(j>=ncols)){
                        continue;//q' is outside of the lattice
                    }
                    if(error[ii*ncols+jj]<=tolerance){//q'= (ii,jj)
                        continue;
                    }
                    //now error[q']>tolerance
                    double stepSize=sqrt(2.0);
                    for(int innerIter=0;innerIter<maxloop;++innerIter){
                        double cr=i+dq[0];
                        double cc=j+dq[1];//g(q)=(cr,cc)
                        for(int signRow=-1;signRow<=1;signRow+=2){
                            for(int signCol=-1;signCol<=1;signCol+=2){
                                double vr=cr+signRow*stepSize*0.5;
                                double vc=cc+signCol*stepSize*0.5;//(vr,vc)=p' (corner of the box )
                                double fvr, fvc;
                                interpolateDisplacementFieldAt(forward, nrows, ncols, vr, vc, fvr, fvc);//f(p') = (vr+fvr, vc+fvc)
                                //find the lattice points q'' surrounding f(p')=(fvr, fvc)
                                //find the top left index and the interpolation coefficients
                                int ii=floor(vr+fvr);
                                int jj=floor(vc+fvc);
                                //assign all grid points immediately adjacent to f(p)
                                for(int k=0;k<4;++k){
                                    int iii=ii+nRow[k];
                                    int jjj=jj+nCol[k];//(iii, jjj) is a corner q'' surrounding f(p')
                                    if((iii<0)||(jjj<0)||(iii>=nrows)||(jjj>=ncols)){
                                        continue;//the corner does not exist
                                    }
                                    double dr=iii-(vr+fvr);
                                    double dc=jjj-(vc+fvc);//(dr, dc) = q''-f(p')
                                    double opt=sqrt(dr*dr+dc*dc);//||q''-f(p')||^2
                                    if(opt<error[iii*ncols+jjj]){
                                        double *dqbiprime=&inv[2*(iii*ncols+jjj)];
                                        dqbiprime[0]=vr-iii;
                                        dqbiprime[1]=vc-jjj;//g(q'')=p' <<==>> q''+inv[q''] = p'  <<==>> inv[q'']=p'-q''
                                        error[iii*ncols+jjj]=opt;
                                    }
                                }//for each q'' surrounding f(p')
                            }
                        }//for each vertex p' of the box G(g(q))
                    }//while innerIter<maxloop
                    
                }//foreach point q' adjacent to q such that e(q')>tolerance
            }
        }//foreach grid point q in S
    }
    delete[] error;
    return 0;
}


//==================================================================================================
//==================================================================================================
//=====================================Yan's algorithm ends=========================================
//==================================================================================================
//==================================================================================================
//==================================================================================================


int invertVectorField_old(double *d, int nrows, int ncols, double lambdaParam, int maxIter, double tolerance, double *invd, double *stats){
    const static int numNeighbors=4;
    const static int dRow[]={-1, 0, 1,  0, -1, 1,  1, -1};
    const static int dCol[]={ 0, 1, 0, -1,  1, 1, -1, -1};
    double *temp=new double[nrows*ncols*2];
    double *denom=new double[nrows*ncols];
    memset(invd, 0, sizeof(double)*nrows*ncols*2);
    double maxChange=tolerance+1;
    int iter;
    for(iter=0;(tolerance*tolerance<maxChange)&&(iter<maxIter);++iter){
        memset(temp, 0, sizeof(double)*nrows*ncols*2);
        memset(denom, 0, sizeof(double)*nrows*ncols);
        double *dx=d;
        for(int i=0;i<nrows;++i){
            for(int j=0;j<ncols;++j, dx+=2){
                for(int k=0;k<numNeighbors;++k){
                    int ii=i+dRow[k];
                    if((ii<0)||(ii>=nrows)){
                        continue;
                    }
                    int jj=j+dCol[k];
                    if((jj<0)||(jj>=ncols)){
                        continue;
                    }
                    denom[i*ncols+j]+=lambdaParam;
                    temp[2*(i*ncols+j)]+=lambdaParam*invd[2*(ii*ncols+jj)];
                    temp[2*(i*ncols+j)+1]+=lambdaParam*invd[2*(ii*ncols+jj)+1];
                }
                //find top-left coordinates
                double dii=i+dx[0];
                double djj=j+dx[1];
                if((dii<0) || (djj<0) || (dii>nrows-1)||(djj>ncols-1)){
                    continue;
                }
                int ii=floor(dx[0]);
                int jj=floor(dx[1]);
                double calpha=dx[0]-ii;//by definition these factors are nonnegative
                double cbeta=dx[1]-jj;
                ii+=i;
                jj+=j;
                double alpha=1-calpha;
                double beta=1-cbeta;
                //top-left corner (x+dx is located at region 1 w.r.t site [ii,jj])
                if((ii>=0)&&(jj>=0)&&(ii<nrows)&&(jj<ncols)){
                    double *z=&temp[2*(ii*ncols+jj)];
                    double &den=denom[ii*ncols+jj];
                    den+=alpha*alpha*beta*beta;
                    z[0]-=alpha*beta*dx[0];
                    z[1]-=alpha*beta*dx[1];
                    if((jj<ncols-1)){//right neighbor
                        double *zright=&invd[2*(ii*ncols+jj+1)];
                        z[0]-=alpha*alpha*beta*cbeta*zright[0];
                        z[1]-=alpha*alpha*beta*cbeta*zright[1];
                    }
                    if((ii<nrows-1)){//bottom neighbor
                        double *zbottom=&invd[2*((ii+1)*ncols+jj)];
                        z[0]-=alpha*calpha*beta*beta*zbottom[0];
                        z[1]-=alpha*calpha*beta*beta*zbottom[1];
                    }
                    if((jj<ncols-1) && (ii<nrows-1)){//bottom right corner
                        double *zbright=&invd[2*((ii+1)*ncols+jj+1)];
                        z[0]-=alpha*calpha*beta*cbeta*zbright[0];
                        z[1]-=alpha*calpha*beta*cbeta*zbright[1];
                    }
                }
                ++jj;
                //top-right corner (x+dx is located at region 2 w.r.t site [ii,jj])
                if((ii>=0)&&(jj>=0)&&(ii<nrows)&&(jj<ncols)){
                    double *z=&temp[2*(ii*ncols+jj)];
                    double &den=denom[ii*ncols+jj];
                    den+=alpha*alpha*cbeta*cbeta;
                    z[0]-=alpha*cbeta*dx[0];
                    z[1]-=alpha*cbeta*dx[1];
                    if((ii<nrows-1)){//bottom neighbor
                        double *zbottom=&invd[2*((ii+1)*ncols+jj)];
                        z[0]-=alpha*calpha*cbeta*cbeta*zbottom[0];
                        z[1]-=alpha*calpha*cbeta*cbeta*zbottom[1];
                    }
                    if((jj>0)){//left neighbor
                        double *zleft=&invd[2*(ii*ncols+jj-1)];
                        z[0]-=alpha*alpha*beta*cbeta*zleft[0];
                        z[1]-=alpha*alpha*beta*cbeta*zleft[1];
                    }
                    if((ii<nrows-1) && (jj>0)){//bottom-left neighbor
                        double *zbleft=&invd[2*((ii+1)*ncols+jj-1)];
                        z[0]-=alpha*calpha*beta*cbeta*zbleft[0];
                        z[1]-=alpha*calpha*beta*cbeta*zbleft[1];
                    }
                }
                ++ii;
                //bottom-right corner (x+dx is located at region 4 w.r.t site [ii,jj])
                if((ii>=0)&&(jj>=0)&&(ii<nrows)&&(jj<ncols)){
                    double *z=&temp[2*(ii*ncols+jj)];
                    double &den=denom[ii*ncols+jj];
                    den+=calpha*calpha*cbeta*cbeta;
                    z[0]-=calpha*cbeta*dx[0];
                    z[1]-=calpha*cbeta*dx[1];
                    if((ii>0)){//top neighbor
                        double *ztop=&invd[2*((ii-1)*ncols+jj)];
                        z[0]-=alpha*calpha*cbeta*cbeta*ztop[0];
                        z[1]-=alpha*calpha*cbeta*cbeta*ztop[1];
                    }
                    if((jj>0)){//left neighbor
                        double *zleft=&invd[2*(ii*ncols+jj-1)];
                        z[0]-=calpha*calpha*beta*cbeta*zleft[0];
                        z[1]-=calpha*calpha*beta*cbeta*zleft[1];
                    }
                    if((ii>0)){//top-left neighbor
                        double *ztleft=&invd[2*((ii-1)*ncols+jj-1)];
                        z[0]-=alpha*calpha*beta*cbeta*ztleft[0];
                        z[1]-=alpha*calpha*beta*cbeta*ztleft[1];
                    }
                }
                --jj;
                //bottom-left corner (x+dx is located at region 3 w.r.t site [ii,jj])
                if((ii>=0)&&(jj>=0)&&(ii<nrows)&&(jj<ncols)){
                    double *z=&temp[2*(ii*ncols+jj)];
                    double &den=denom[ii*ncols+jj];
                    den+=calpha*calpha*beta*beta;
                    z[0]-=calpha*beta*dx[0];
                    z[1]-=calpha*beta*dx[1];
                    if((ii>0)){//top neighbor
                        double *ztop=&invd[2*((ii-1)*ncols+jj)];
                        z[0]-=alpha*calpha*beta*beta*ztop[0];
                        z[1]-=alpha*calpha*beta*beta*ztop[1];
                    }
                    if((jj<ncols-1)){//right neighbor
                        double *zright=&invd[2*(ii*ncols+jj+1)];
                        z[0]-=calpha*calpha*beta*cbeta*zright[0];
                        z[1]-=calpha*calpha*beta*cbeta*zright[1];
                    }
                    if((ii>0)&&(jj<ncols-1)){//top-right neighbor
                        double *ztright=&invd[2*((ii-1)*ncols+jj+1)];
                        z[0]-=alpha*calpha*beta*cbeta*ztright[0];
                        z[1]-=alpha*calpha*beta*cbeta*ztright[1];
                    }//if
                }///if
            }//for ncols
        }//for nrows
        //update the inverse
        double *id=invd;
        double *tmp=temp;
        double *den=denom;
        maxChange=0;
        for(int i=0;i<nrows;++i){
            for(int j=0;j<ncols;++j, id+=2, tmp+=2, den++){
                tmp[0]/=(*den);
                tmp[1]/=(*den);
                double nrm=(tmp[0]-id[0])*(tmp[0]-id[0])+(tmp[1]-id[1])*(tmp[1]-id[1]);
                if(maxChange<nrm){
                    maxChange=nrm;
                }
                id[0]=tmp[0];
                id[1]=tmp[1];
            }
        }
    }//for iter
    delete[] temp;
    delete[] denom;
    stats[0]=sqrt(maxChange);
    stats[1]=iter;
    return 0;
}

/*
    Computes comp(x)=d2(d1(x))+d1(x) (i.e. applies first d1, then d2 to the result)
*/
int composeVectorFields(double *d1, double *d2, int nrows, int ncols, double *comp, double *stats){
    double *dx=d1;
    double *res=comp;
    double maxNorm=0;
    double meanNorm=0;
    double stdNorm=0;
    int cnt=0;
    memset(comp, 0, sizeof(double)*nrows*ncols*2); 
    for(int i=0;i<nrows;++i){
        for(int j=0;j<ncols;++j, dx+=2, res+=2){
            int ii=floor(dx[0]);
            int jj=floor(dx[1]);
            double calpha=dx[0]-ii;//by definition these factors are nonnegative
            double cbeta=dx[1]-jj;
            double alpha=1-calpha;
            double beta=1-cbeta;
            //---top-left
            ii+=i;
            jj+=j;
            if((ii<0)||(jj<0)||(ii>=nrows)||(jj>=ncols)){
                continue;
            }
            res[0]=dx[0];
            res[1]=dx[1];
            double *z=&d2[2*(ii*ncols+jj)];
            res[0]+=alpha*beta*z[0];
            res[1]+=alpha*beta*z[1];
            //---top-right
            ++jj;
            if(jj<ncols){
                z=&d2[2*(ii*ncols+jj)];
                res[0]+=alpha*cbeta*z[0];
                res[1]+=alpha*cbeta*z[1];
            }
            //---bottom-right
            ++ii;
            if((ii>=0)&&(jj>=0)&&(ii<nrows)&&(jj<ncols)){
                z=&d2[2*(ii*ncols+jj)];
                res[0]+=calpha*cbeta*z[0];
                res[1]+=calpha*cbeta*z[1];
            }
            //---bottom-left
            --jj;
            if((ii>=0)&&(jj>=0)&&(ii<nrows)&&(jj<ncols)){
                z=&d2[2*(ii*ncols+jj)];
                res[0]+=calpha*beta*z[0];
                res[1]+=calpha*beta*z[1];
            }
            //consider only displacements that land inside the image
            if((i+dx[0]>=0 && i+dx[0]<=nrows-1) && (j+dx[1]>=0 && j+dx[1]<=ncols-1)){
                double nn=res[0]*res[0]+res[1]*res[1];
                if(maxNorm<nn){
                    maxNorm=nn;
                }
                meanNorm+=nn;
                stdNorm+=nn*nn;
                ++cnt;
            }
        }
    }
    meanNorm/=cnt;
    stats[0]=sqrt(maxNorm);
    stats[1]=sqrt(meanNorm);
    stats[2]=sqrt(stdNorm/cnt - meanNorm*meanNorm);
    return 0;
}

/*
    Interpolates the vector field d2 at d1: d2(d1(x)) (i.e. applies first d1, then d2 to the result)
    Seen as a linear operator, it is defined by d1 and the input vector is d2
*/
int vectorFieldInterpolation(double *d1, double *d2, int nrows, int ncols, double *comp){
    double *dx=d1;
    double *res=comp;
    memset(comp, 0, sizeof(double)*nrows*ncols*2); 
    for(int i=0;i<nrows;++i){
        for(int j=0;j<ncols;++j, dx+=2, res+=2){
            double dii=i+dx[0];
            double djj=j+dx[1];
            if((dii<0)||(djj<0)||(dii>nrows-1)||(djj>ncols-1)){
                continue;
            }
            int ii=floor(dii);
            int jj=floor(djj);
            double calpha=dii-ii;//by definition these factors are nonnegative
            double cbeta=djj-jj;
            double alpha=1-calpha;
            double beta=1-cbeta;
            //---top-left
            double *z=&d2[2*(ii*ncols+jj)];
            res[0]+=alpha*beta*z[0];
            res[1]+=alpha*beta*z[1];
            //---top-right
            ++jj;
            if(jj<ncols){
                z=&d2[2*(ii*ncols+jj)];
                res[0]+=alpha*cbeta*z[0];
                res[1]+=alpha*cbeta*z[1];
            }
            //---bottom-right
            ++ii;
            if((ii>=0)&&(jj>=0)&&(ii<nrows)&&(jj<ncols)){
                z=&d2[2*(ii*ncols+jj)];
                res[0]+=calpha*cbeta*z[0];
                res[1]+=calpha*cbeta*z[1];
            }
            //---bottom-left
            --jj;
            if((ii>=0)&&(jj>=0)&&(ii<nrows)&&(jj<ncols)){
                z=&d2[2*(ii*ncols+jj)];
                res[0]+=calpha*beta*z[0];
                res[1]+=calpha*beta*z[1];
            }
        }
    }
    return 0;
}

/*
    Vector field interpolation taking as input the displacements from two separate arrays
*/
int vectorFieldInterpolation(double *d1r, double *d1c, double *d2r, double *d2c, int nrows, int ncols, double *compr, double *compc){
    double *dr=d1r;
    double *dc=d1c;
    double *resr=compr;
    double *resc=compc;
    memset(compr, 0, sizeof(double)*nrows*ncols); 
    memset(compc, 0, sizeof(double)*nrows*ncols); 
    for(int i=0;i<nrows;++i){
        for(int j=0;j<ncols;++j, ++dr, ++dc, ++resr, ++resc){
            double dii=i+*dr;
            double djj=j+*dc;
            if((dii<0)||(djj<0)||(dii>nrows-1)||(djj>ncols-1)){
                continue;
            }
            int ii=floor(dii);
            int jj=floor(djj);
            double calpha=dii-ii;//by definition these factors are nonnegative
            double cbeta=djj-jj;
            double alpha=1-calpha;
            double beta=1-cbeta;
            //---top-left
            double *zr=&d2r[ii*ncols+jj];
            double *zc=&d2c[ii*ncols+jj];
            (*resr)+=alpha*beta*(*zr);
            (*resc)+=alpha*beta*(*zc);
            //---top-right
            ++jj;
            if(jj<ncols){
                zr=&d2r[ii*ncols+jj];
                zc=&d2c[ii*ncols+jj];
                (*resr)+=alpha*cbeta*(*zr);
                (*resc)+=alpha*cbeta*(*zc);
            }
            //---bottom-right
            ++ii;
            if((ii>=0)&&(jj>=0)&&(ii<nrows)&&(jj<ncols)){
                zr=&d2r[ii*ncols+jj];
                zc=&d2c[ii*ncols+jj];
                (*resr)+=calpha*cbeta*(*zr);
                (*resc)+=calpha*cbeta*(*zc);
            }
            //---bottom-left
            --jj;
            if((ii>=0)&&(jj>=0)&&(ii<nrows)&&(jj<ncols)){
                zr=&d2r[ii*ncols+jj];
                zc=&d2c[ii*ncols+jj];
                (*resr)+=calpha*beta*(*zr);
                (*resc)+=calpha*beta*(*zc);
            }
        }
    }
    return 0;
}


//Computes the Adjoint transformation associated to the interpolation transformation defined by d1, applied to d2.
//i.e. d1 defines the transformation and d2 is the input vector
int vectorFieldAdjointInterpolation(double *d1, double *d2, int nrows, int ncols, double *sol){
    double *dx=d1;
    double *z=d2;
    memset(sol, 0, sizeof(double)*nrows*ncols*2); 
    for(int i=0;i<nrows;++i){
        for(int j=0;j<ncols;++j, dx+=2, z+=2){
            double dii=i+dx[0];
            double djj=j+dx[1];
            if((dii<0)||(djj<0)||(dii>nrows-1)||(djj>ncols-1)){
                continue;
            }
            int ii=floor(dii);
            int jj=floor(djj);
            double calpha=dii-ii;//by definition these factors are nonnegative
            double cbeta=djj-jj;
            double alpha=1-calpha;
            double beta=1-cbeta;
            //top left
            double *res=&sol[2*(ii*ncols+jj)];
            res[0]+=alpha*beta*z[0];
            res[1]+=alpha*beta*z[1];
            //---top-right
            ++jj;
            if(jj<ncols){
                double *res=&sol[2*(ii*ncols+jj)];
                res[0]+=alpha*cbeta*z[0];
                res[1]+=alpha*cbeta*z[1];
            }
            //---bottom-right
            ++ii;
            if((ii>=0)&&(jj>=0)&&(ii<nrows)&&(jj<ncols)){
                double *res=&sol[2*(ii*ncols+jj)];
                res[0]+=calpha*cbeta*z[0];
                res[1]+=calpha*cbeta*z[1];
            }
            //---bottom-left
            --jj;
            if((ii>=0)&&(jj>=0)&&(ii<nrows)&&(jj<ncols)){
                double *res=&sol[2*(ii*ncols+jj)];
                res[0]+=calpha*beta*z[0];
                res[1]+=calpha*beta*z[1];
            }
        }
    }
    return 0;
}

/*
    Vector field adjoint interpolation taking as input the displacements from two separate arrays
*/
int vectorFieldAdjointInterpolation(double *d1r, double *d1c, double *d2r, double *d2c, int nrows, int ncols, double *solr, double *solc){
    double *dr=d1r;
    double *dc=d1c;
    double *zr=d2r;
    double *zc=d2c;
    memset(solr, 0, sizeof(double)*nrows*ncols); 
    memset(solc, 0, sizeof(double)*nrows*ncols); 
    for(int i=0;i<nrows;++i){
        for(int j=0;j<ncols;++j, ++dr, ++dc, ++zr, ++zc){
            double dii=i+(*dr);
            double djj=j+(*dc);
            if((dii<0)||(djj<0)||(dii>nrows-1)||(djj>ncols-1)){
                continue;
            }
            int ii=floor(dii);
            int jj=floor(djj);
            double calpha=dii-ii;//by definition these factors are nonnegative
            double cbeta=djj-jj;
            double alpha=1-calpha;
            double beta=1-cbeta;
            //top left
            double *resr=&solr[ii*ncols+jj];
            double *resc=&solc[ii*ncols+jj];
            (*resr)+=alpha*beta*(*zr);
            (*resc)+=alpha*beta*(*zc);
            //---top-right
            ++jj;
            if(jj<ncols){
                resr=&solr[ii*ncols+jj];
                resc=&solc[ii*ncols+jj];
                (*resr)+=alpha*cbeta*(*zr);
                (*resc)+=alpha*cbeta*(*zc);
            }
            //---bottom-right
            ++ii;
            if((ii>=0)&&(jj>=0)&&(ii<nrows)&&(jj<ncols)){
                resr=&solr[ii*ncols+jj];
                resc=&solc[ii*ncols+jj];
                (*resr)+=calpha*cbeta*(*zr);
                (*resc)+=calpha*cbeta*(*zc);
            }
            //---bottom-left
            --jj;
            if((ii>=0)&&(jj>=0)&&(ii<nrows)&&(jj<ncols)){
                resr=&solr[ii*ncols+jj];
                resc=&solc[ii*ncols+jj];
                (*resr)+=calpha*beta*(*zr);
                (*resc)+=calpha*beta*(*zc);
            }
        }
    }
    return 0;
}

int invertVectorField_TV_L2(double *forward, int nrows, int ncols, double lambdaParam, int maxIter, double tolerance, double *inv){
    int nsites=nrows*ncols;
    double *fr=new double[nsites];
    double *fc=new double[nsites];
    for(int i=0;i<nsites;++i){
        fr[i]=forward[2*i];
        fc[i]=forward[2*i+1];
    }
    double *tmpr=new double[nsites];
    double *tmpc=new double[nsites];
    double *tmpr2=new double[nsites];
    double *tmpc2=new double[nsites];
    double *prr=new double[nsites];
    double *prc=new double[nsites];
    double *pcr=new double[nsites];
    double *pcc=new double[nsites];
    double *qr=new double[nsites];
    double *qc=new double[nsites];
    double *sbarr=new double[nsites];
    double *sbarc=new double[nsites];
    double *sr=new double[nsites];
    double *sc=new double[nsites];
    double L=8;
    double sigma=1.0/L;
    double tau=sigma;
    double theta=0.8;
    double error=1+tolerance;
    int iter=0;
    memset(sbarr, 0, sizeof(double)*nrows*ncols);//initialize the inverse field
    memset(sbarc, 0, sizeof(double)*nrows*ncols);
    memset(sr, 0, sizeof(double)*nrows*ncols);//initialize the inverse field
    memset(sc, 0, sizeof(double)*nrows*ncols);
    memset(prr, 0, sizeof(double)*nrows*ncols);//initialize the dual Jacobian
    memset(prc, 0, sizeof(double)*nrows*ncols);
    memset(pcr, 0, sizeof(double)*nrows*ncols);
    memset(pcc, 0, sizeof(double)*nrows*ncols);
    memset(qr, 0, sizeof(double)*nrows*ncols);
    memset(qc, 0, sizeof(double)*nrows*ncols);
    double factorq=lambdaParam/(lambdaParam+sigma);
    //FILE *F=fopen("TVL2iterations.txt", "w");
    while((tolerance<error) && (iter<=maxIter)){
        ++iter;
        //update the dual p-variables
        computeGradient(sbarr, nrows, ncols, tmpr, tmpc);
        for(int i=nsites-1;i>=0;--i){
            prr[i]+=sigma*tmpr[i];
            prc[i]+=sigma*tmpc[i];
        }
        computeGradient(sbarc, nrows, ncols, tmpr, tmpc);
        for(int i=nsites-1;i>=0;--i){
            pcr[i]+=sigma*tmpr[i];
            pcc[i]+=sigma*tmpc[i];
            double nrm=(prr[i]*prr[i])+(prc[i]*prc[i])+(pcr[i]*pcr[i])+(pcc[i]*pcc[i]);
            if(nrm>1){
                nrm=sqrt(nrm);
                prr[i]/=nrm;
                prc[i]/=nrm;
                pcr[i]/=nrm;
                pcc[i]/=nrm;
            }
        }
        //update the dual q-variables
        vectorFieldInterpolation(fr, fc, sbarr, sbarc, nrows, ncols, tmpr, tmpc);
        for(int i=nsites-1;i>=0;--i){
            qr[i]=(qr[i]+sigma*(tmpr[i]+fr[i]))*factorq;
            qc[i]=(qc[i]+sigma*(tmpc[i]+fc[i]))*factorq;
        }
        //update primal variables and step
        vectorFieldAdjointInterpolation(fr, fc, qr, qc, nrows, ncols, tmpr, tmpc);
        for(int i=nsites-1;i>=0;--i){
            sbarr[i]=-theta*sr[i];//save -theta times the previous value of s
            sbarc[i]=-theta*sc[i];
            sr[i]-=tau*tmpr[i];
            sc[i]-=tau*tmpc[i];
            
        }
        computeDivergence(prr, prc, nrows, ncols, tmpr);
        computeDivergence(pcr, pcc, nrows, ncols, tmpc);
        for(int i=nsites-1;i>=0;--i){
            sr[i]+=tau*tmpr[i];
            sc[i]+=tau*tmpc[i];
        }
        //finish comuting the s-bar step
        for(int i=nsites-1;i>=0;--i){
            sbarr[i]+=(1.0+theta)*sr[i];
            sbarc[i]+=(1.0+theta)*sc[i];
        }
        //----compute error----
        double newError=0;
        vectorFieldInterpolation(fr, fc, sr, sc, nrows, ncols, tmpr, tmpc);
        for(int i=0;i<nsites;++i){
            double dr=tmpr[i]+fr[i];
            double dc=tmpc[i]+fc[i];
            newError+=dr*dr+dc*dc;
        }
        newError*=lambdaParam;
        computeGradient(sr, nrows, ncols, tmpr, tmpc);
        computeGradient(sc, nrows, ncols, tmpr2, tmpc2);
        for(int i=0;i<nsites;++i){
            newError+=sqrt(tmpr[i]*tmpr[i]+tmpc[i]*tmpc[i]+tmpr2[i]*tmpr2[i]+tmpc2[i]*tmpc2[i]);
        }
        //fprintf(F, "%d: %e\n", iter, newError);
        error=fabs(error-newError);
    }
    //fclose(F);
    double *g=inv;
    for(int i=0;i<nsites;++i, g+=2){
        g[0]=sr[i];
        g[1]=sc[i];
    }
    delete[] tmpr;
    delete[] tmpc;
    delete[] tmpr2;
    delete[] tmpc2;
    delete[] fr;
    delete[] fc;
    delete[] prr;
    delete[] prc;
    delete[] pcr;
    delete[] pcc;
    delete[] qr;
    delete[] qc;
    delete[] sbarr;
    delete[] sbarc;
    delete[] sr;
    delete[] sc;
    return 0;
}


int invertVectorFieldFixedPoint(double *d, int nrows, int ncols, int maxIter, double tolerance, double *invd, double *stats){
    double error=1+tolerance;
    double *temp[2];
    temp[0]=new double[nrows*ncols*2];
    temp[1]=invd;
    memset(temp[0], 0, sizeof(double)*2*nrows*ncols);
    int nsites=2*nrows*ncols;
    int iter;
    for(iter=0;(iter<maxIter) && (tolerance<error);++iter){
        composeVectorFields(temp[iter&1], d, nrows, ncols, temp[1-(iter&1)], stats);
        double *p=temp[1-(iter&1)];
        for(int i=0;i<nsites;++i,++p){
            *p*=-1;
        }
        composeVectorFields(d, temp[1-(iter&1)], nrows, ncols, temp[1-(iter&1)], stats);
        error=stats[1];
    }
    if(iter&1){//then the last computation was stored at temp[0]
        memcpy(invd, temp[0], sizeof(double)*2*nrows*ncols);
    }
    delete[] temp[0];
    return 0;
}

int vectorFieldExponential(double *v, int nrows, int ncols, double *expv, double *invexpv){
    double EXP_EPSILON=0.01;//such that the vector field exponential is approx the identity
    //---compute the maximum norm---
    double stats[3];
    int nsites=nrows*ncols;
    double maxNorm=0;
    double *d=v;
    for(int i=0;i<nsites;++i, d+=2){
        double nn=d[0]*d[0]+d[1]*d[1];
        if(maxNorm<nn){
            maxNorm=nn;
        }
    }
    maxNorm=sqrt(maxNorm);
    int n=0;
    double factor=1.0;
    while(EXP_EPSILON<maxNorm){
        maxNorm*=0.5;
        factor*=0.5;
        ++n;
    }
    //---base case---
    if(n<1){
        memcpy(expv, v, sizeof(double)*2*nsites);
        invertVectorField(v, nrows, ncols, 0.5, 100, 1e-4, invexpv, stats);
        return 0;
    }
    //---prepare memory buffers---
    double *tmp[2]={NULL, NULL};
    tmp[n%2]=new double[nsites*2];
    tmp[1-n%2]=expv;
    //---perform binary exponentiation: exponential---
    for(int i=2*nsites-1;i>=0;--i){
        tmp[1][i]=v[i]*factor;
    }
    for(int i=1;i<=n;++i){
        composeVectorFields(tmp[i&1], tmp[i&1], nrows, ncols, tmp[1-(i&1)], stats);
    }
    //---perform binary exponentiation: inverse---
    tmp[1-n%2]=invexpv;
    for(int i=2*nsites-1;i>=0;--i){
        tmp[0][i]=v[i]*factor;
    }
    invertVectorField(tmp[0], nrows, ncols, 0.1, 20, 1e-4, tmp[1], stats);
    for(int i=1;i<=n;++i){
        composeVectorFields(tmp[i&1], tmp[i&1], nrows, ncols, tmp[1-(i&1)], stats);
    }
    delete[] tmp[n%2];
    return 0;
}


int writeDoubleBuffer(double *buffer, int nDoubles, char *fname){
    FILE *F=fopen(fname, "wb");
    fwrite(buffer, sizeof(double), nDoubles, F);
    fclose(F);
    return 0;
}

int readDoubleBuffer(char *fname, int nDoubles, double *buffer){
    FILE *F=fopen(fname, "rb");
    fread(buffer, sizeof(double), nDoubles, F);
    fclose(F);
    return 0;
}


void createInvertibleDisplacementField(int nrows, int ncols, double b, double m, double *dField){
    int midRow=nrows/2;
    int midCol=ncols/2;
    double *d=dField;
    for(int i=0;i<nrows;++i){
        for(int j=0;j<ncols;++j, d+=2){
            int ii=i-midRow;
            int jj=j-midCol;
            double theta=atan2(ii,jj);
            d[0]=ii*(1.0/(1+b*cos(m*theta))-1.0);
            d[1]=jj*(1.0/(1+b*cos(m*theta))-1.0);
        }
    }
}


void countSupportingDataPerPixel(double *forward, int nrows, int ncols, int *counts){
    memset(counts, 0, sizeof(int)*nrows*ncols);
    double *f=forward;
    for(int i=0;i<nrows;++i){
        for(int j=0;j<ncols;++j, f+=2){
            double di=i+f[0];
            double dj=j+f[1];
            if(di<0 || dj<0 || di>nrows-1 || dj>ncols-1){
                continue;
            }
            int ii=floor(i+f[0]);
            int jj=floor(j+f[1]);
            counts[ii*ncols+jj]++;
            ++jj;
            if(jj<ncols){
                counts[ii*ncols+jj]++;
            }
            ++ii;
            if(jj<ncols && ii<nrows){
                counts[ii*ncols+jj]++;
            }
            --jj;
            if(ii<nrows){
                counts[ii*ncols+jj]++;
            }
            
        }
    }
}
