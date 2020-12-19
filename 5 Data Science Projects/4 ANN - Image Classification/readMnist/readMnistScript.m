[train,train_out,valid,valid_out,test,test_out]=readMNIST('train_img.idx3-ubyte', 'train_label.idx1-ubyte', 'test_img.idx3-ubyte', 'test_label.idx1-ubyte', 20000, 5000, 10000);

dlmwrite('train_data.txt',[train',train_out]);
dlmwrite('valid_data.txt',[valid',valid_out]);
dlmwrite('test_data.txt',[test',test_out]);