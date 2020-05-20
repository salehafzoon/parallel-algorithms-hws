#include<stdio.h>
#include<stdlib.h>
#include<cuda.h>

#define TILE_WIDTH 100 // for shared kernel
#define DEBUG 0
typedef double Type;

void transpose_CPU(Type* in_, Type* out, int dim1, int dim2){
		
		for(int n = 0; n<dim1*dim2; n++) {
        int i = n/dim1;
        int j = n%dim1;
        out[n] = in_[dim2*j + i];
    }
}

//transpose per element
__global__ void transpose_GPU(Type* in_, Type* out, int dim1, int dim2){

	int tile_size = blockDim.x ;
	int column = tile_size * blockIdx.x + threadIdx.x;
	int row = tile_size * blockIdx.y + threadIdx.y;

	if(column < dim2 && row < dim1){
		out[column*dim2 + row] = in_[column + row*dim2];
	}
}

// SHARED MEM APROACH - use shared memory
__global__ void transpose_GPU_shared(Type* in_, Type* out, int dim1, int dim2){

	// fill data into shared memory
	__shared__ Type M_Shared[TILE_WIDTH][TILE_WIDTH];

	int tile_size =TILE_WIDTH;
	int column = tile_size * blockIdx.x + threadIdx.x;
	int row = tile_size * blockIdx.y + threadIdx.y;
	int index_in = row*dim2 + column;
	int index_out = column*dim2 + row;


	if(row < dim1 && column < dim2 && index_in < dim1*dim2){
		M_Shared[threadIdx.y][threadIdx.x] = in_[index_in];
	}
	__syncthreads(); // wait all other threads to go further.

	if(row < dim1 && column < dim2 && index_out < dim1*dim2){
		out[index_out] = M_Shared[threadIdx.y][threadIdx.x];
	}
}

void print_matrix(Type * mat, int dim1, int dim2){
	for (int i = 0; i < dim1; i++) {
		for (int j = 0; j < dim2; j++) {
			Type num = mat[i + dim2 * j];
			printf(" %.2f ",num);
		}
		printf("\n");
	}
}
void initial_matirx(Type * mat, int dim1, int dim2){
		for (int i = 0; i < dim1*dim2; ++i) {
			mat[i] = i;
	}
}

int main(int argc , char* argv[]){

	clock_t ser_msec;
	int dim1 =  atoi(argv[1]);
	int dim2 =  atoi(argv[2]);
	
	printf("Matrix data type : double\n ");
	printf("dimentions = (%d , %d) ,Tile width = %d",dim1,dim2,TILE_WIDTH);
	
	int size = dim1*dim2*sizeof(Type);

	Type *in_ = (Type*)malloc(size);
	Type *out = (Type*)malloc(size);
	Type *ref = (Type*)malloc(size);
	
	// init matrix
	initial_matirx(in_,dim1,dim2);

	Type *d_in , *d_out;

	cudaMalloc(&d_in, size);
	cudaMalloc(&d_out, size);
	cudaMemcpyAsync(d_in, in_, size, cudaMemcpyHostToDevice);
	cudaMalloc((Type**)&d_out,size);
	cudaMemset(d_out,0,size);

	// init kernel
	int threadNumX = TILE_WIDTH;
	int threadNumY = TILE_WIDTH;
	int blockNumX = dim1 / TILE_WIDTH + (dim1 % TILE_WIDTH == 0 ? 0 : 1 );
	int blockNumY = dim2 / TILE_WIDTH + (dim2 % TILE_WIDTH == 0 ? 0 : 1 );

	dim3 blockSize(threadNumX,threadNumY);
	dim3 gridSize(blockNumX, blockNumY);

	cudaEvent_t start, stop,start_shared, stop_shared;
	float elapsedTime, elapsedTime2;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	cudaEventCreate(&start_shared);
	cudaEventCreate(&stop_shared);

	if(DEBUG && dim1<50 && dim2<50 ){
		printf("\n*********matrix**********\n");
		print_matrix(in_, dim1, dim2);
	}

	ser_msec = clock();
	transpose_CPU(in_, ref, dim1, dim2);
	ser_msec = (double)(clock() - ser_msec) * 1000000 /CLOCKS_PER_SEC;

	if(DEBUG && dim1<50 && dim2<50 ){	
			printf("\n*********serial**********\n");
			print_matrix(ref, dim2, dim1);
	}
	
	cudaEventRecord(start, 0);
	transpose_GPU<<<gridSize,blockSize>>>(d_in,d_out,dim1,dim2);
	cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&elapsedTime, start, stop);

	cudaMemcpy(out,d_out,size,cudaMemcpyDeviceToHost);

	cudaEventRecord(start_shared, 0);
	transpose_GPU_shared<<<gridSize,blockSize>>>(d_in,d_out,dim1,dim2);
	cudaEventRecord(stop_shared, 0);
	cudaEventSynchronize(stop_shared);
	cudaEventElapsedTime(&elapsedTime2, start_shared, stop_shared);

	cudaMemcpy(out,d_out,size,cudaMemcpyDeviceToHost);

	if(DEBUG && dim1<50 && dim2<50 ){
		printf("\n*********parallel**********\n");
		print_matrix(out, dim2, dim1);
	}
	
	printf ("\nTime for the serial: \t %ld ms", ser_msec);
	printf ("\nTime for the NAIVE: \t %f ms and speedup: %f", elapsedTime, ser_msec/elapsedTime);
	printf ("\nTime for the shared: \t %f ms and speedup: %f \n\n", elapsedTime2, ser_msec/elapsedTime2);
	

	// cleanup
	cudaFree(d_in);
  cudaFree(d_out);
	free(in_);
	free(out);
	free(ref);

	return 0;
}