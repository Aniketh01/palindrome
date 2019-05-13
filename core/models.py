# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models

# Create your models here.

class IP(models.Model):
    ipaddr = models.CharField(max_length=50)

    def __str__(self):
        return self.ipaddr