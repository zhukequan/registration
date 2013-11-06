# -*- coding: utf-8 -*-
"""
Created on Mon Sep 30 15:40:28 2013

@author: khayyam
"""
import numpy as np
import scipy as sp
from scipy import ndimage
import matplotlib.pyplot as plt
import nibabel as nib
###############################################################
##########################  Common  ###########################
###############################################################
const_prefilter_map_coordinates=False

def getDistribution(img1, img2):
    sh=img1.shape
    dist=np.zeros((256,256))
    for i in range(sh[0]):
        for j in range(sh[1]):
            a=int(img1[i,j])
            b=int(img2[i,j])
            dist[a,b]+=1
    return dist

def drawLattice2D(nrows, ncols, delta):
    lattice=np.ndarray((1+(delta+1)*nrows, 1+(delta+1)*ncols), dtype=np.float64)
    lattice[...]=127
    for i in range(nrows+1):
        lattice[i*(delta+1), :]=0
    for j in range(ncols+1):
        lattice[:, j*(delta+1)]=0
    return lattice

def createDeformationField2D_type1(nrows, ncols, maxDistp):
    deff=np.ndarray((nrows, ncols, 2), dtype=np.float64)
    midCol=ncols//2
    midRow=nrows//2
    for i in range(nrows):
        deff[i,:,0]=maxDistp*np.sin(2*np.pi*(np.array(range(ncols), dtype=np.float64)-midCol)/ncols)
    for j in range(ncols):
        deff[:,j,1]=maxDistp*np.sin(2*np.pi*(np.array(range(nrows), dtype=np.float64)-midRow)/nrows)
    v=np.array(range(nrows), dtype=np.float64)-midRow
    h=np.array(range(ncols), dtype=np.float64)-midCol
    nrm=midRow**2+midCol**2
    p=np.exp(-(v[:,None]**2+h[None,:]**2)/(0.1*nrm))
    p=(p-p.min())/(p.max()-p.min())
    deff[:,:,0]*=p
    deff[:,:,1]*=p
    return deff

def createDeformationField2D_type2(nrows, ncols, maxDistp):
    deff=np.ndarray((nrows, ncols, 2), dtype=np.float64)
    midCol=ncols//2
    midRow=nrows//2
    for i in range(nrows):
        deff[i,:,0]=maxDistp*np.sin(2*np.pi*(np.array(range(ncols), dtype=np.float64)-midCol)/ncols)
        deff[i,:,1]=maxDistp*np.sin(2*np.pi*(np.array(range(ncols), dtype=np.float64)-midCol)/ncols)
    for j in range(ncols):
        deff[:,j,0]*=np.sin(2*np.pi*(np.array(range(nrows), dtype=np.float64)-midRow)/nrows)
        deff[:,j,1]*=np.sin(2*np.pi*(np.array(range(nrows), dtype=np.float64)-midRow)/nrows)
    return deff

def createDeformationField2D_type3(nrows, ncols, maxDistp):
    deff=np.ndarray((nrows, ncols, 2), dtype=np.float64)
    X0,X1=np.mgrid[0:nrows, 0:ncols]
    midCol=ncols//2
    midRow=nrows//2
    nn=np.sqrt(midCol*midCol+midRow*midRow)
    factor=maxDistp/nn
    deff[...,0]=(X1-midCol)*(-factor)
    deff[...,1]=(X0-midRow)*(factor)
    return deff

def drawLattice3D(dims, delta):
    lattice=np.ndarray((1+(delta+1)*dims[0], 1+(delta+1)*dims[1], 1+(delta+1)*dims[2]), dtype=np.float64)
    lattice[...]=127
    for i in range(dims[0]+1):
        lattice[i*(delta+1), :, :]=0
    for j in range(dims[1]+1):
        lattice[:, j*(delta+1), :]=0
    for k in range(dims[2]+1):
        lattice[:, :, k*(delta+1)]=0
    return lattice

def createDeformationField3D_type2(dims, maxDistp):
    deff=np.ndarray(dims+(3,), dtype=np.float64)
    dims=np.array(dims, dtype=np.int32)
    mid=dims//2
    factor=maxDistp**(1.0/3.0)
    for i in range(dims[0]):
        deff[i,:,:,0]=factor*np.sin(2*np.pi*(np.array(range(dims[0]), dtype=np.float64)-mid[0])/dims[0])
        deff[i,:,:,1]=factor*np.sin(2*np.pi*(np.array(range(dims[0]), dtype=np.float64)-mid[0])/dims[0])
        deff[i,:,:,2]=factor*np.sin(2*np.pi*(np.array(range(dims[0]), dtype=np.float64)-mid[0])/dims[0])
    for j in range(dims[1]):
        deff[:,j,:,0]*=factor*np.sin(2*np.pi*(np.array(range(dims[1]), dtype=np.float64)-mid[1])/dims[1])
        deff[:,j,:,1]*=factor*np.sin(2*np.pi*(np.array(range(dims[1]), dtype=np.float64)-mid[1])/dims[1])
        deff[:,j,:,2]*=factor*np.sin(2*np.pi*(np.array(range(dims[1]), dtype=np.float64)-mid[1])/dims[1])
    for k in range(dims[2]):
        deff[:,:,k,0]*=factor*np.sin(2*np.pi*(np.array(range(dims[2]), dtype=np.float64)-mid[2])/dims[2])
        deff[:,:,k,1]*=factor*np.sin(2*np.pi*(np.array(range(dims[2]), dtype=np.float64)-mid[2])/dims[2])
        deff[:,:,k,2]*=factor*np.sin(2*np.pi*(np.array(range(dims[2]), dtype=np.float64)-mid[2])/dims[2])
    return deff

    
def warpImage(image, displacement):
    sh=image.shape
    X0,X1=np.mgrid[0:sh[0], 0:sh[1]]
    warped=ndimage.map_coordinates(image, [X0+displacement[...,0], X1+displacement[...,1]], prefilter=const_prefilter_map_coordinates)
    return warped

def warpVolume(volume, displacement):
    sh=volume.shape
    X0,X1,X2=np.mgrid[0:sh[0], 0:sh[1], 0:sh[2]]
    warped=ndimage.map_coordinates(volume, [X0+displacement[...,0], X1+displacement[...,1], X2+displacement[...,2]], prefilter=const_prefilter_map_coordinates)
    return warped

def plotDeformationField(d):
    plt.figure()
    plt.quiver(d[...,1], d[...,0])

def plotDeformationFields(dList):
    n=len(dList)
    plt.figure()
    for i in range(n):
        plt.subplot(1,n,i+1)
        plt.quiver(dList[i][...,1], dList[i][...,0])

def plotOrthogonalField(sh, b):
    center=(np.array(sh)-1)/2.0
    C,R=sp.meshgrid(np.array(range(sh[1]), dtype=np.float64), np.array(range(sh[0]), dtype=np.float64))
    R=R-center[0]+b[0]
    C=C-center[1]+b[1]
    plt.figure()
    plt.quiver(R, -C)

def plotPyramids(L, R):
    n=len(L)
    plt.figure()
    for i in range(n):
        plt.subplot(2,n,i+1)
        plt.imshow(L[i], cmap = plt.cm.gray)
        plt.title('Level: '+str(i))
        plt.subplot(2,n,n+i+1)
        plt.imshow(R[i], cmap = plt.cm.gray)
        plt.title('Level: '+str(i))

def renormalizeImage(image):
    m=np.min(image)
    M=np.max(image)
    if(M-m<1e-8):
        return image
    return 127.0*(image-m)/(M-m)

def plotOverlaidPyramids(L, R):
    n=len(L)
    plt.figure()
    for i in range(n):
        plt.subplot(1,n,i+1)
        colorImage=np.zeros(shape=(L[i].shape)+(3,), dtype=np.int8)
        ll=renormalizeImage(L[i]).astype(np.int8)
        rr=renormalizeImage(R[i]).astype(np.int8)
        colorImage[...,0]=ll*(ll>ll[0,0])
        colorImage[...,1]=rr*(rr>rr[0,0])
        plt.imshow(colorImage)
        plt.title('Level: '+str(i))
        
def plotOverlaidPyramids3DCoronal(L, R):
    n=len(L)
    plt.figure()
    for i in range(n):
        sh=L[i].shape
        plt.subplot(1,n,i+1)
        colorImage=np.zeros(shape=(sh[0], sh[2], 3), dtype=np.int8)
        ll=renormalizeImage(L[i][:,sh[1]//2,:]).astype(np.int8)
        rr=renormalizeImage(R[i][:,sh[1]//2,:]).astype(np.int8)
        colorImage[...,0]=ll*(ll>ll[0,0])
        colorImage[...,1]=rr*(rr>rr[0,0])
        plt.imshow(colorImage)
        plt.title('Level: '+str(i))

def getRotationMatrix(angles):
    ca=np.cos(angles[0])
    cb=np.cos(angles[1])
    cg=np.cos(angles[2])
    sa=np.sin(angles[0])
    sb=np.sin(angles[1])
    sg=np.sin(angles[2])
    return np.array([[cb*cg,-ca*sg+sa*sb*cg,sa*sg+ca*sb*cg],[cb*sg,ca*cg+sa*sb*sg,-sa*cg+ca*sb*sg],[-sb,sa*cb,ca*cb]])

def getRotationMatrix2D(angle):
    c=np.cos(angle)
    s=np.sin(angle)
    return np.array([[c, -s],[s, c]])


def applyRigidTransformation3D(image, beta):
    sh=image.shape
    center=(np.array(sh)-1)/2.0
    X0,X1,X2=np.mgrid[0:sh[0], 0:sh[1], 0:sh[2]]
    X0=X0-center[0]
    X1=X1-center[1]
    X2=X2-center[2]
    R=getRotationMatrix(beta[0:3])
    X0new,X1new,X2new=(R[0,0]*X0 + R[0,1]*X1 + R[0,2]*X2 + center[0] + beta[3], 
                       R[1,0]*X0 + R[1,1]*X1 + R[1,2]*X2 + center[1] + beta[4], 
                       R[2,0]*X0 + R[2,1]*X1 + R[2,2]*X2 + center[2] + beta[5])
    return ndimage.map_coordinates(image, [X0new, X1new, X2new], prefilter=const_prefilter_map_coordinates)

def pyramid_gaussian_3D(image, max_layer, mask=None):
    yield image.copy().astype(np.float64)
    for i in range(max_layer):
        newImage=np.empty(shape=((image.shape[0]+1)//2, (image.shape[1]+1)//2, (image.shape[2]+1)//2), dtype=np.float64)
        newImage[...]=sp.ndimage.filters.gaussian_filter(image, 2.0/3.0)[::2,::2,::2]
        if(mask!=None):
            mask=mask[::2,::2,::2]
            newImage*=mask
        image=newImage
        yield newImage

def pyramid_gaussian_2D(image, max_layer, mask=None):
    yield image.copy().astype(np.float64)
    for i in range(max_layer):
        newImage=np.empty(shape=((image.shape[0]+1)//2, (image.shape[1]+1)//2), dtype=np.float64)
        newImage[...]=sp.ndimage.filters.gaussian_filter(image, 2.0/3.0)[::2,::2]
        if(mask!=None):
            mask=mask[::2,::2]
            newImage*=mask
        image=newImage
        yield newImage

def overlayImages(img0, img1, createFig=True):
    colorImage=np.zeros(shape=(img0.shape)+(3,), dtype=np.int8)
    colorImage[...,0]=renormalizeImage(img0)
    colorImage[...,1]=renormalizeImage(img1)
    fig=None
    if(createFig):
        fig=plt.figure()
    plt.imshow(colorImage)
    return fig

def testOverlayImages_raw():
    leftName='data/t2/t2_icbm_normal_1mm_pn0_rf0.rawb'
    rightName='data/t1/t1_icbm_normal_1mm_pn0_rf0.rawb'
    ns=181
    nr=217
    nc=181
    left=np.fromfile(leftName, dtype=np.ubyte).reshape(ns,nr,nc)
    left=left.astype(np.float64)
    right=np.fromfile(rightName, dtype=np.ubyte).reshape(ns,nr,nc)
    right=right.astype(np.float64)
    overlayImages(left[90], right[90])

def testOverlayImages_nii():
    leftName='data/t1/IBSR18/IBSR_01/IBSR_01_ana_strip.nii.gz'
    rightName='data/t1/IBSR18/IBSR_02/IBSR_02_ana_strip.nii.gz'
    nib_left = nib.load(leftName)
    nib_right = nib.load(rightName)
    left=nib_left.get_data().astype(np.double)
    right=nib_right.get_data().astype(np.double)
    sl=np.array(left.shape)//2
    sr=np.array(right.shape)//2
    overlayImages(left[sl[0],:,:,0], right[sr[0],:,:,0])
    overlayImages(left[:,sl[1],:,0], right[:,sr[1],:,0])
    overlayImages(left[:,:,sl[2],0], right[:,:,sr[2],0])