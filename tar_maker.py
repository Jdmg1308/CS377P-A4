import tarfile
with tarfile.open('A4.tar', 'w') as tar:
    tar.add('folder/')