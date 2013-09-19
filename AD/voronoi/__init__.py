try:
    from pkgutil import extend_path
    __path__ = extend_path(__path__, __name__)

    from sys import path
    from os.path import abspath

    path.append(abspath(".."))
    print path
except:
    raise EnvironmentError("Can't append voronoi to python path :/ ")
