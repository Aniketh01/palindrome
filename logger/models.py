# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models

# Create your models here.s
class loggerModel(models.Model):
    loggerBuffer = models.CharField(max_length=50)

    def __str__(self):
        return self.loggerBuffer