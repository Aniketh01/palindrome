from django import forms
from logger.models import loggerModel

class loggerForm(forms.ModelForm):
    loggerBuf = forms.CharField()

    class Meta:
        model = loggerModel
        fields = ('loggerBuf', )