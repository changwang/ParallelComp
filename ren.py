import os

CONF_PATH = '/Users/changwang/opensource/hadoop/conf'

def switch_conf(mode):
    if mode == 'pseudo':
        os.rename('/Users/changwang/opensource/hadoop/conf/core-site.xml', 
            '/Users/changwang/opensource/hadoop/conf/core-site pseudo.xml')
        os.rename('/Users/changwang/opensource/hadoop/conf/core-site standalone.xml', 
            '/Users/changwang/opensource/hadoop/conf/core-site.xml')
        
        os.rename('/Users/changwang/opensource/hadoop/conf/hdfs-site.xml', 
            '/Users/changwang/opensource/hadoop/conf/hdfs-site pseudo.xml')
        os.rename('/Users/changwang/opensource/hadoop/conf/hdfs-site standalone.xml', 
            '/Users/changwang/opensource/hadoop/conf/hdfs-site.xml')
            
        os.rename('/Users/changwang/opensource/hadoop/conf/mapred-site.xml', 
            '/Users/changwang/opensource/hadoop/conf/mapred-site pseudo.xml')
        os.rename('/Users/changwang/opensource/hadoop/conf/mapred-site standalone.xml', 
            '/Users/changwang/opensource/hadoop/conf/mapred-site.xml')
        
        print "Configuration switched from pesudo to standalone"
    elif mode == 'standalone':
        os.rename('/Users/changwang/opensource/hadoop/conf/core-site.xml', 
            '/Users/changwang/opensource/hadoop/conf/core-site standalone.xml')
        os.rename('/Users/changwang/opensource/hadoop/conf/core-site pseudo.xml', 
            '/Users/changwang/opensource/hadoop/conf/core-site.xml')
            
        os.rename('/Users/changwang/opensource/hadoop/conf/hdfs-site.xml', 
            '/Users/changwang/opensource/hadoop/conf/hdfs-site standalone.xml')
        os.rename('/Users/changwang/opensource/hadoop/conf/hdfs-site pseudo.xml', 
            '/Users/changwang/opensource/hadoop/conf/hdfs-site.xml')
        
        os.rename('/Users/changwang/opensource/hadoop/conf/mapred-site.xml', 
            '/Users/changwang/opensource/hadoop/conf/mapred-site standalone.xml')
        os.rename('/Users/changwang/opensource/hadoop/conf/mapred-site pseudo.xml', 
            '/Users/changwang/opensource/hadoop/conf/mapred-site.xml')
        
        print "Configuration switched from standalone to pseudo"
    

if __name__ == '__main__':
    path = os.path.join(CONF_PATH, 'mode.txt')
    fmode = open(path, 'r+')
    line = fmode.readline()
    fmode.close()
    
    fmode = open(path, 'w+')
    
    switch_conf(line)

    if line == 'standalone':
        fmode.write('pseudo')
    else:
        fmode.write('standalone')
    fmode.flush()
    fmode.close()