

__kernel void reinforecment_learning(const int M, const int N, const int K,
                      const __global float* Q_old,
                      const __global float* maxQ,
		      const float learning_rate,
		      const __global float* reward,
		      const float discount_factor,
                      __global float* Q_new) {
    
    // Thread identifiers
    const int row = get_local_id(0); 
    const int col = get_local_id(1); 

    for (int s_t=0; s_t<row;s_t++)
    {
      float estimate_of_futre_value = maxQ[0];
      for (int a_t=0; a_t <col; a_t++){
 	if (maxQ[a_t]> estimate_of_futre_value)
	{
		estimate_of_futre_value = maxQ[a_t];
	}	 
      }
     Q_new[s_t] =  (1-learning_rate) * Q_old[s_t] + learning_rate * (reward[s_t] + discount_factor *  estimate_of_futre_value);
    }
}

