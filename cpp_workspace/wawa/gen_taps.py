#!/bin/python

import numpy as np
import scipy as sp
import matplotlib.pyplot as plt

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


  if isinstance(Fc, (list, tuple, np.ndarray)) == False:
    Fc_list = [Fc]
  else:
    Fc_list = Fc


  if isinstance(Q, (list, tuple, np.ndarray)) == False:
    Q_list = [Q for i in range(len(Fc))]
  else:
    Q_list = Q


  if isinstance(G, (list, tuple, np.ndarray)) == False:
    G_list = [G for i in range(len(Fc))]
  else:
    G_list = G

  if isinstance(Type, (list, tuple, np.ndarray)) == False:
    Type_list = [Type for i in range(len(Fc))]
  else:
    Type_list = Type


  if len(Fc_list) != len(Type_list) or \
     len(Fc_list) != len(G_list) or \
     len(Fc_list) != len(Q_list):

    print("Error: List lengths differ")

    print("len(Fc_list)   : %i" % len(Fc_list))
    print("len(Type_list) : %i" % len(Type_list))
    print("len(G_list)    : %i" % len(G_list))
    print("len(Q_list)    : %i" % len(Q_list))

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
#  Fc_start = 20
#  Fc_step = 10
#  N = 256
#  Fc = np.arange(Fc_start, Fc_start+N*Fc_step, Fc_step, dtype=np.float64)
#
#  k = 0.5
#  Q_min = 0.027
#  Q_max = 2.0
#  Q = 10**(1-k*(np.arange(0, N, 1, dtype=float)/N))/10
#  Q = 1-Q
#  Q = Q / ((np.max(Q)-np.min(Q)))
#  Q = Q * (Q_max-Q_min) + Q_min

  Fc_start = 1100
  Fc_step = 15
  N = 256
  Fc = np.arange(Fc_start, Fc_start+N*Fc_step, Fc_step, dtype=np.float64)

  Q = 4.0

  G = 20 # dB
  Type = ["low" for i in range(256)]


  output_dir = "."

  main(Fc, Q, G, Type, output_dir)

  filter_bank = {}

  filter_bank["b_int"] = np.fromfile("./b_taps.bin", dtype=np.int64)
  filter_bank["a_int"] = np.fromfile("./a_taps.bin", dtype=np.int64)

  post_filter_gain = 0 # dB

  filter_bank["b"] = np.zeros([256, 3], dtype=np.float64)
  filter_bank["a"] = np.zeros([256, 3], dtype=np.float64)
  for i in range(256):
    filter_bank["b"][i] = np.float64(filter_bank["b_int"][i*3:i*3+3] / 2**61)
    filter_bank["a"][i] = np.float64(filter_bank["a_int"][i*3:i*3+3] / 2**61)

  filter_bank["b"] = filter_bank["b"] * 10**(post_filter_gain/20)

  x = np.zeros(2**14, dtype=np.float64)
  x[256] = 1.0

  w = np.arange(0, 1, 1/len(x), dtype=float)*48

  plt.figure()
  plt.xlabel("Frequency (kHz)")
  plt.ylabel("Gain (dB)")
  plt.grid()
  for i in np.arange(0, 256, 16, dtype=int):
    conv_o = sp.signal.lfilter(filter_bank["b"][i], filter_bank["a"][i], x)
    fft_out = 20*np.log10(np.abs((sp.fft.fft(conv_o))))
    plt.plot(w[0:int(len(w)/2)], fft_out[0:int(len(w)/2)])

  plt.show()


