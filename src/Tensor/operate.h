	/*----------------加法 [ add + ]----------------*/
	Tensor& add(Tensor& a, Tensor& b) {
		if (!(a.dim == b.dim)) error();
		alloc(a.dim);
		for (int i = 0; i < b.size(); i++) data[i] = a[i] + b[i];
		return *this;
	}
	Tensor& operator+=(Tensor& a) {
		if (!(dim == a.dim)) error();
		for (int i = 0; i < size(); i++) data[i] += a[i];
		return *this;
	}
	/*----------------数乘 [ mul × ]----------------*/
	Tensor& mul(const double a, Tensor& b) {
		alloc(b.dim);
		for (int i = 0; i < b.size(); i++) data[i] = a * b[i];
		return *this;
	}
	Tensor& operator*=(const double a) {
		for (int i = 0; i < size(); i++) data[i] *= a;
		return *this;
	}
	/*----------------卷积----------------*/
	Tensor& conv(Tensor& in, Tensor& kernel, int padding, int stride) {
		alloc(
			(in.dim[0] - kernel.dim[0] + 2 * padding) / stride + 1,
			(in.dim[1] - kernel.dim[1] + 2 * padding) / stride + 1,
			kernel.dim[2]
		);
		// for each element of out
		for (int i = 0; i < size(); i++)							// for each element of kernel
			for (int ky = 0; ky < kernel.dim[1]; ky++) 
				for (int kx = 0; kx < kernel.dim[0]; kx++) {			// get the corresponding element of in
					int xt = -padding + i2x(i) * stride + kx,
						yt = -padding + i2y(i) * stride + ky;
					if (xt < 0 || xt >= in.dim[0] || yt < 0 || yt >= in.dim[1]) continue;
					for (int z = 0; z < in.dim[2]; z++)
						data[i] += in(xt, yt, z) * kernel(kx, ky, i2z(i));
				}
	}
	/*----------------合并 [ merge ]----------------
	*	比 dimIndex 阶数低级的，作为元素块，整体进行内存复制
		比 dimIndex 阶数高级的，作为复制次数
	-----------------------------------------------*/
	Tensor& merge(Tensor* a[], const int N, int dimIndex) {
		// new memory
		int length = 0;
		for (int i = 0; i < N; i++) length += a[i]->dim[dimIndex];
		Mat<int> dimSize = a[0]->dim;
		dimSize[dimIndex] = length;
		Tensor ansTemp(dimSize.rows, dimSize.data);
		// merge
		int elementBlockSize = 1, copyTimes = 1;
		for (int i = 0; i < dimIndex; i++)    elementBlockSize *= dim[i];
		for (int i = dimIndex + 1; i < dim.rows; i++)copyTimes *= dim[i];
		int pos = 0;
		Mat<int> inputPosMem(N, 1);
		while (copyTimes--) {
			for (int i = 0; i < N; i++) {
				int t = elementBlockSize * a[i]->dim[dimIndex];
				memcpy((&data + pos), &(a[i]->data) + inputPosMem[i], t * sizeof(T));
				pos += t; inputPosMem[i] += t;
			}
		}
		eat(ansTemp); return *this;
	}
	/*----------------函数操作 []----------------*/
	template<typename F>
	Tensor& function(Tensor& x, F&& f) {
		alloc(x.dim);
		for (int i = 0; i < x.size(); i++) data[i] = f(x[i]);
		return *this;
	}
	template<typename F>
	Tensor& function(F&& f) {
		for (int i = 0; i <   size(); i++) data[i] = f(data[i]);
		return *this;
	}