function [train,train_out,valid,valid_out,test,test_out] = readMNIST(train_img,train_label,test_img, test_label,train_line,valid_line,test_line)
%to run: 
%[train,train_out,valid,valid_out,test,test_out]=readMNIST('train_img.idx3-ubyte', 'train_label.idx1-ubyte', 'test_img.idx3-ubyte', 'test_label.idx1-ubyte', 50000, 10000, 10000);
    % Read training and validation digits
    fid = fopen(train_img, 'r', 'b');
    header = fread(fid, 1, 'int32');
    if header ~= 2051
        error('Invalid image file header');
    end
    count = fread(fid, 1, 'int32');
    if count < train_line+valid_line
        error('Trying to read too many digits from train_img file');
    end
    
    r = fread(fid, 1, 'int32');         %row number
    c = fread(fid, 1, 'int32');         %col number
     
    train = zeros(r,c,train_line);
    
    for i=1:train_line
        for y=1:c
            train(:,y,i) = fread(fid, r, 'uint8');
        end
    end
    
    valid = zeros(r,c,valid_line);
    
    for i=1:valid_line
        for y=1:c
            valid(:,y,i) = fread(fid, r, 'uint8');
        end
    end
    
    fclose(fid);
    
    % Read training and validation labels
    fid = fopen(train_label, 'r', 'b');
    header = fread(fid, 1, 'int32');
    if header ~= 2049
        error('Invalid label file header');
    end
    count = fread(fid, 1, 'int32');
    if count < train_line+valid_line
        error('Trying to read too many digits from train_img file');
    end
    
    train_out = fread(fid, train_line, 'uint8');
    valid_out=fread(fid, valid_line, 'uint8');
    fclose(fid);
    
    % Read test digits
    fid = fopen(test_img, 'r', 'b');
    header = fread(fid, 1, 'int32');
    if header ~= 2051
        error('Invalid test image file header');
    end
    count = fread(fid, 1, 'int32');
    if count < test_line
        error('Trying to read too many digits from test_img file');
    end
    
    r = fread(fid, 1, 'int32');         %row number
    c = fread(fid, 1, 'int32');         %col number
     
    test = zeros(r,c,test_line);
    
    for i=1:test_line
        for y=1:c
            train(:,y,i) = fread(fid, r, 'uint8');
        end
    end
    
    fclose(fid);
    
    % Read test labels
    fid = fopen(test_label, 'r', 'b');
    header = fread(fid, 1, 'int32');
    if header ~= 2049
        error('Invalid test label file header');
    end
    count = fread(fid, 1, 'int32');
    if count < test_line
        error('Trying to read too many digits from test_img file');
    end
    
    test_out = fread(fid, test_line, 'uint8');
    fclose(fid);
    
    %train = trimDigits(train, 4);
    %valid = trimDigits(valid, 4);
    %test=trimDigits(test,4);
    train = normalizePixValue(train);
    valid = normalizePixValue(valid);
    test=normalizePixValue(test);
    
    train=reshape(train,784,train_line);
    valid=reshape(valid,784,valid_line);
    test=reshape(test,784,test_line);
    
end

function digits = trimDigits(digitsIn, border)
    dSize = size(digitsIn);
    digits = zeros([dSize(1)-(border*2) dSize(2)-(border*2) dSize(3)]);
    for i=1:dSize(3)
        digits(:,:,i) = digitsIn(border+1:dSize(1)-border, border+1:dSize(2)-border, i);
    end
end

function digits = normalizePixValue(digits)
    digits = double(digits);
    for i=1:size(digits, 3)
        digits(:,:,i) = digits(:,:,i)./255.0;
    end
end

