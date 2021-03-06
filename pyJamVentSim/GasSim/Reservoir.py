#
# resovoire with a fixed size...
#

import pprint as pp
import math
from .SimNode import SimNode, NodeType
from . import Const





class Reservoir(SimNode):
    def __init__(self, name, pressure, volume):
        SimNode.__init__(self, name, NodeType.RESEROVIRE);
        self._next_out.pressure = pressure;
        self._next_out.volume = volume;

    def step(self, dt):
        SimNode.step(self,dt)

        # avoid dealing with both the pressure AND temperature chaning
        #    I am assuming an adbadiac conditions, to avoid a having to
        #      solve 2 ordanary differential equations...

        # keep this simple for each device connected to us for now
        #   assume a single source formula instead of parallel
        #   sources from different pressures...

        # for each connection figure out how much is entering or leaving in mols
        numValveOpen=0;
        Pdelta=0;
        ppNewO2=0;
        for c in self._connections:
            if not (c.out.open):
                continue;
            numValveOpen += 1;
            #
            #              1                # calculate the new pressure..
            # P      = (-----------)        #   this is only valid for a single
            #               dt/RV           #   inlet at a time... no parallel inlets...
            #             e
            Pdrop = c.getPressureDrop(self);
            R=c.out.resistance;
            V=self.out.volume;
            tc=1-math.exp(-dt/(R*V))
            Pdelta=Pdrop*(1-math.exp(-dt/(R*V)))      # this is not handling multiple circuits open.
            ppNewO2+=Pdelta*c.out.pO2;
            # print("name={}: c={}, c.out.pO2={:.6}, ppNewO2={:.6}".format(self.name(),
            #                                                              c.name(),
            #                                                              c.out.
            #                                                              pO2,ppNewO2))

        assert(numValveOpen <= 1);      # don't support parallel filling of the container yet...
        # ttry working with partial pressures.
        ppO2 = self.out.pressure * self.out.pO2
        ppO2 += ppNewO2;
        self._next_out.pressure = self.out.pressure + Pdelta;
        self._next_out.pO2=ppO2/self._next_out.pressure;

        assert(round(self._next_out.pO2,2) <= 1.0);
        # this check is unique to this model, nO2 can never be zero or we did
        #  something very wrong.
        assert (self._next_out.pO2 > 0);

    def getValues(self):
        '''
        retrieve the values for this node, and only this node in format ready to do a debug print.
        :return: print string.
        '''
        return("{}: P={:1.6g}, pN2={:1.6g}, pO2={:1.6g}".format(self.getNodeType(),
                                                                self.out.pressure, self.out.pN2, self.out.pO2));













