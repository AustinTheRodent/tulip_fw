#!/bin/python

import numpy as np

# Function to calculate biquad coefficients for each band
def calc_biquad(frequency, filter_type, gain, fs, Q):
  A = 10**(gain / 40)  # Linear gain
  w0 = 2 * np.pi * frequency / fs  # Normalized frequency
  alpha = np.sin(w0) / (2 * Q)

  if filter_type == 'peak':
    # Peaking EQ filter
    b0 = 1 + alpha * A
    b1 = -2 * np.cos(w0)
    b2 = 1 - alpha * A
    a0 = 1 + alpha / A
    a1 = -2 * np.cos(w0)
    a2 = 1 - alpha / A

  elif filter_type == 'low':
    # Low-shelf filter
    b0 = A * ((A + 1) - (A - 1) * np.cos(w0) + 2 * np.sqrt(A) * alpha)
    b1 = 2 * A * ((A - 1) - (A + 1) * np.cos(w0))
    b2 = A * ((A + 1) - (A - 1) * np.cos(w0) - 2 * np.sqrt(A) * alpha)
    a0 = (A + 1) + (A - 1) * np.cos(w0) + 2 * np.sqrt(A) * alpha
    a1 = -2 * ((A - 1) + (A + 1) * np.cos(w0))
    a2 = (A + 1) + (A - 1) * np.cos(w0) - 2 * np.sqrt(A) * alpha

  elif filter_type == 'high':
    # High-shelf filter
    b0 = A * ((A + 1) + (A - 1) * np.cos(w0) + 2 * np.sqrt(A) * alpha)
    b1 = -2 * A * ((A - 1) + (A + 1) * np.cos(w0))
    b2 = A * ((A + 1) + (A - 1) * np.cos(w0) - 2 * np.sqrt(A) * alpha)
    a0 = (A + 1) - (A - 1) * np.cos(w0) + 2 * np.sqrt(A) * alpha
    a1 = 2 * ((A - 1) - (A + 1) * np.cos(w0))
    a2 = (A + 1) - (A - 1) * np.cos(w0) - 2 * np.sqrt(A) * alpha

  return np.array([b0 / a0, b1 / a0, b2 / a0]), np.array([1, a1 / a0, a2 / a0])


def main(Fc, Q, G, Type, output_dir, Fs=48000):

  IIR_TAP_DEC_BITS = 62
  IIR_TAP_INT_BITS = 2
  IIR_TAP_DEPTH = 3


  try:
    if len(Fc) == 1:
      Fc_list = [Fc[0] for i in range(len(Fc))]
    else:
      Fc_list = Fc
  except:
    Fc_list = [Fc for i in range(len(Fc))]


  try:
    if len(Q) == 1:
      Q_list = [Q[0] for i in range(len(Fc))]
    else:
      Q_list = Q
  except:
    Q_list = [Q for i in range(len(Fc))]


  try:
    if len(G) == 1:
      G_list = [G[0] for i in range(len(Fc))]
    else:
      G_list = G
  except:
    G_list = [G for i in range(len(Fc))]


  try:
    if len(Type) == 1:
      Type_list = [Type[0] for i in range(len(Fc))]
    else:
      Type_list = Type
  except:
    Type_list = [Type for i in range(len(Fc))]


  if len(Fc_list) != len(Type_list) or \
     len(Fc_list) != len(G_list) or \
     len(Fc_list) != len(Q_list):
    print("Error: List lengths differ")
    return

  b_taps_array = np.zeros(len(Fc_list)*IIR_TAP_DEPTH, dtype=np.int64)
  b_taps_counter = 0
  a_taps_array = np.zeros(len(Fc_list)*IIR_TAP_DEPTH, dtype=np.int64)
  a_taps_counter = 0

  for i in range(len(Fc_list)):
    b, a = calc_biquad(Fc_list[i], Type_list[i], G_list[i], Fs, Q_list[i])

    for tap in b:
      b_taps_array[b_taps_counter] = np.int64(tap * 2**(IIR_TAP_DEC_BITS-1) / (10**(G_list[i]/20)))
      b_taps_counter += 1

    for tap in a:
      a_taps_array[a_taps_counter] = np.int64(tap * 2**(IIR_TAP_DEC_BITS-1))
      a_taps_counter += 1

  b_taps_array.tofile(output_dir + "/b_taps.bin")
  a_taps_array.tofile(output_dir + "/a_taps.bin")

if __name__ == "__main__":
  Fc_start = 20
  Fc_step = 10
  N = 256

  Q = 0.5
  G = 20 # dB
  Type = "peak"
  output_dir = "."

  Fc = np.arange(Fc_start, Fc_start+N*Fc_step, Fc_step, dtype=np.float64)
  main(Fc, Q, G, Type, output_dir)



